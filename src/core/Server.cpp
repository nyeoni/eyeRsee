
#include "core/Server.hpp"

#include <sys/socket.h>
#include <unistd.h>  // close

#include <iostream>

#include "core/Type.hpp"
#include "core/Udata.hpp"
#include "entity/Client.hpp"
#include "handler/ResponseHandler.hpp"

namespace ft {

void Env::parse(int argc, char **argv) {
    double d_port;
    char *back;
    std::string port_str;

    if (argc != 3) throw std::logic_error("invalid argc error");
    port_str = argv[1];
    if (port_str.find('.') != std::string::npos)
        throw std::logic_error(
            "Error: arguments\n[hint] ./ft_irc <port(1025 ~ 65535)>");
    d_port = std::strtod(port_str.c_str(), &back);
    if (*back || d_port < 1025 | d_port > 65535) {
        throw std::logic_error(
            "Error: arguments\n[hint] ./ft_irc <port(1025 ~ 65535)>");
    }
    port = static_cast<int>(d_port);
    // TODO password rule (max_len)
    password = argv[2];
}

/****************************************************/
/*                     Server                       */
/****************************************************/

Server::Server() {
    //_connect_socket = new ConnectSocket[256];
    // TODO max_client 할지 말지 여부
    //    _socket_list.reserve(ft::Env::max_client);
}
Server::~Server() {
    // if (_connect_socket) delete[] _connect_socket;
}

void Server::init(int argc, char **argv) { _env.parse(argc, argv); }

// socket
void Server::run() {
    // 1. server socket
    _listen_socket.createSocket(_env.port);  // env.port
    std::cout << "🚀 Server running listening on port " << _env.port
              << std::endl;
    registerEvent(_listen_socket.getFd(), EVFILT_READ, ACCEPT, 0);

    while (true) {
        int n = monitorEvent();
        for (int i = 0; i < n; i++) {
            handleEvent(i);
        }
        garbageCollector();
    }
}

/****************************************************/
/*                   EventHandler                   */
/****************************************************/

void Server::handleAccept() {
    std::cout << "Accept" << std::endl;
    Client *new_client;
    Udata *udata;
    int connected_fd;

    new_client = _executor.accept(_listen_socket.getFd());

    udata = new Udata;
    udata->src = new_client;
    connected_fd = new_client->getFd();
    _udata_list.insert(std::make_pair(connected_fd, udata));

    registerEvent(connected_fd, EVFILT_READ, READ, udata);
    registerEvent(connected_fd, EVFILT_TIMER, TIMER, udata);
}

void Server::handleRead(int event_idx) {
    std::cout << "==== Read ====" << std::endl;

    char buf[BUF_SIZE];
    ssize_t n = 0;
    Event event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);
    ConnectSocket *connect_socket = udata->src;

    n = recv(event.ident, &buf, BUF_SIZE, 0);
    if (n == -1) {
        _garbage.insert(udata);
        return;
    }
    buf[n] = 0;

    connect_socket->recv_buf.append(buf);

    // parse commandlines in connect_sockets
    udata->commands = _parser.parse(udata->src);

    if (udata->src->status == REGISTER) {
        if (!udata->commands.empty()) {
            registerEvent(event.ident, EVFILT_WRITE, EXECUTE, udata);
        }
    } else if (udata->src->status == UNREGISTER) {
        std::vector<Command *>::iterator iter = udata->commands.begin();
        for (; iter != udata->commands.end(); ++iter) {
            _executor.connect(*iter, udata->src, _env.password);
        }
        udata->commands.clear();

        // check is authenticate
        if (connect_socket->isAuthenticate()) {
            std::cout << "Register Success!" << std::endl;
            connect_socket->status = REGISTER;
            _tmp_garbage.erase(udata);

            ResponseHandler::handleConnectResponse(udata->src);
            // TODO check
            response(connect_socket->getFd(), connect_socket->send_buf);
        }
    }
}

void Server::handleExecute(int event_idx) {
    std::cout << "==== Execute ====" << event_idx << std::endl;

    Event &event = _ev_list[event_idx];

    Udata *udata = static_cast<Udata *>(event.udata);
    Client *client = udata->src;

    std::vector<Command *>::iterator iter = udata->commands.begin();
    for (; iter != udata->commands.end(); ++iter) {
        _executor.execute(*iter, client);
    }
    udata->commands.clear();

    const std::set<int> &fd_list = _executor.getFdList();
    std::set<int>::iterator fd = fd_list.begin();
    for (; fd != fd_list.end(); ++fd) {
        registerEvent(*fd, EVFILT_WRITE, EXECUTE,
                      _udata_list.find(*fd)->second);
    }
    _executor.clearFdLIst();

    if (response(event.ident, udata->src->send_buf) == 0)
        registerEvent(event.ident, EVFILT_WRITE, D_WRITE, 0);
}

void Server::handleTimer(int event_idx) {
    Event &event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);
    ConnectSocket *socket = udata->src;

    registerEvent(event.ident, EVFILT_TIMER, D_TIMER, 0);
    if (socket->isAuthenticate()) {
        return;
    }
    _tmp_garbage.insert(udata);
}
void Server::handleTimeout() {
    std::set<Udata *>::iterator iter = _tmp_garbage.begin();

    for (; iter != _tmp_garbage.end(); ++iter) {
        _garbage.insert(*iter);
        send((*iter)->src->getFd(), "Timeout\n", 9, 0);
    }
    iter = _garbage.begin();
    for (; iter != _garbage.end(); ++iter) {
        _tmp_garbage.erase(*iter);
    }
}

void Server::handleClose() {
    std::set<Udata *>::iterator iter = _garbage.begin();
    for (; iter != _garbage.end(); ++iter) {
        Client *client = (*iter)->src;
        int connected_fd = client->getFd();
        std::cout << "New client # " << connected_fd << " gone" << std::endl;
        client->deleteSocket();
        _executor.deleteClient(client);
        _udata_list.erase(connected_fd);
        delete (*iter);
    }
    _garbage.clear();
}

int Server::response(int fd, std::string &send_buf) {
    ssize_t n;
    n = send(fd, send_buf.c_str(), send_buf.length(), 0);
    if (n == send_buf.length()) {
        send_buf.clear();
        return 0;
    }
    if (n > 0)  // TODO
        send_buf = send_buf.substr(n, send_buf.length());
    else
        std::cerr << "[UB] send return -1" << std::endl;
    return -1;
}

}  // namespace ft
