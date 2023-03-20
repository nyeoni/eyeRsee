
#include "core/Server.hpp"

#include <sys/socket.h>
#include <unistd.h>  // close

#include <iostream>
#include <sstream>

#include "core/Type.hpp"
#include "core/Udata.hpp"
#include "core/utility.hpp"
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
    ConnectSocket new_socket;
    Client *new_client;
    Udata *udata;

    new_socket.createSocket(_listen_socket.getFd());
    new_client = _executor.createClient(new_socket.getFd());
    new_client->setFd(new_socket.getFd());

    udata = new Udata;
    udata->src = new_client;

    registerEvent(new_client->getFd(), EVFILT_READ, CONNECT, udata);
    registerEvent(new_client->getFd(), EVFILT_TIMER, TIMER, udata);
}

void Server::handleConnect(int event_idx) {
    std::cout << "Connect" << std::endl;

    char buf[BUF_SIZE];
    ssize_t n;
    Event event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);
    Client *new_client = udata->src;

    // read connect_socket
    n = recv(event.ident, &buf, BUF_SIZE, 0);
    buf[n] = 0;

    new_client->recv_buf.append(buf);

    // multi commands parse
    std::string line = new_client->readRecvBuf();
    std::vector<std::string> command_lines = split(line, '\n');
    std::vector<std::string>::iterator it;
    for (it = command_lines.begin(); it != command_lines.end(); it++) {
        Command *command = new Command;

        _parser.parse(*it, command->type, command->params);
        udata->commands.push_back(command);
    }

    // connect to clients logic
    // authenticate error
    std::vector<Command *>::iterator iter = udata->commands.begin();
    for (; iter != udata->commands.end(); ++iter) {
        _executor.connect(*iter, new_client, _env.password);
    }
    udata->commands.clear();

    // check is authenticate
    if (new_client->isAuthenticate()) {
        _rsv_garbage.erase(udata);
        send(event.ident, WELCOME_PROMPT, strlen(WELCOME_PROMPT), 0);
        registerEvent(event.ident, EVFILT_READ, READ, udata);
        std::cout << "#" << event.ident << "READ event registered!"
                  << std::endl;
    }
}

void Server::handleRead(int event_idx) {
    std::cout << "==== Read ====" << std::endl;

    char buf[BUF_SIZE];
    ssize_t n = 0;
    Event event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);
    Client *connect_socket = udata->src;

    // read connect_socket
    n = recv(event.ident, &buf, BUF_SIZE, 0);
    if (n == -1) {
        perror("handleRead (recv -1): ");
        return;
    }
    buf[n] = 0;

    connect_socket->recv_buf.append(buf);

    // multi commands parse
    std::string line = connect_socket->readRecvBuf();
    std::vector<std::string> command_lines = split(line, '\n');
    std::vector<std::string>::iterator it;
    for (it = command_lines.begin(); it != command_lines.end(); it++) {
        Command *command = new Command;
        try {
            _parser.parse(*it, command->type, command->params);
            udata->commands.push_back(command);
        } catch (std::exception &e) {
//            ErrorHandler::handleError(e);
            // ErrorHandler::handler
            delete command;
        }
    }

    // registerRead
    if (command_lines.size())
        registerEvent(event.ident, EVFILT_WRITE, EXECUTE, udata);
}

void Server::handleExecute(int event_idx) {
    std::cout << "execute " << event_idx << std::endl;
    Event &event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);
    Client *client = udata->src;

    std::vector<Command *>::iterator iter = udata->commands.begin();
    for (; iter != udata->commands.end(); ++iter) {
        _executor.execute(*iter, client);
    }
    udata->commands.clear();

    if (response(event.ident, udata->src->send_buf) == 0)
        registerEvent(event.ident, EVFILT_WRITE, D_WRITE, 0);
    else
        registerEvent(event.ident, EVFILT_WRITE, WRITE, udata);
}

void Server::handleWrite(int event_idx) {
    Event &event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);

    std::cout << "write # " << event.ident << std::endl;
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
    _rsv_garbage.insert(udata);
}
void Server::handleTimeout() {
    std::vector<Udata *> tmp;  // iterator 생명주기..
    std::set<Udata *>::iterator iter = _rsv_garbage.begin();

    tmp.reserve(_rsv_garbage.size());
    for (; iter != _rsv_garbage.end(); ++iter) {
        if (getTicks() > (*iter)->src->create_time + 5000) {
            _garbage.insert(*iter);
            tmp.push_back(*iter);
            send((*iter)->src->getFd(), "Timeout\n", 9, 0);

            // std::cout << "New client # " << (*iter)->src->getFd()
            //           << " timeout for register" << std::endl;
        }
    }
    if (tmp.size()) {
        std::vector<Udata *>::iterator tmp_iter = tmp.begin();
        for (; tmp_iter != tmp.end(); ++tmp_iter) {
            _rsv_garbage.erase(*tmp_iter);
        }
    }
}

void Server::handleClose() {
    std::set<Udata *>::iterator iter = _garbage.begin();
    for (; iter != _garbage.end(); ++iter) {
        Client *client = (*iter)->src;
        std::cout << "New client # " << client->getFd() << " gone" << std::endl;
        client->deleteSocket();
        _executor.deleteClient(client);
        delete (*iter);
    }
    _garbage.clear();
}

bool Server::isConnected(Udata *udata) {
    return _executor.isConnected(udata->src);
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
