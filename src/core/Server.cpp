
#include "core/Server.hpp"

#include <sys/socket.h>
#include <unistd.h>  // close

#include <iostream>

#include "core/Command.hpp"
#include "core/Type.hpp"
#include "core/utility.hpp"
#include "entity/Client.hpp"
#include "handler/ErrorHandler.hpp"
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
    int connected_fd;

    new_client = _executor.accept(_listen_socket.getFd());

    connected_fd = new_client->getFd();

    registerEvent(connected_fd, EVFILT_READ, READ, new_client);
    registerEvent(connected_fd, EVFILT_TIMER, TIMER, new_client);
}

void Server::handleRead(int event_idx) {
    std::cout << "==== Read ====" << std::endl;
    Event event = _ev_list[event_idx];
    Client *client = static_cast<Client *>(event.udata);
    ConnectSocket *connect_socket = static_cast<ConnectSocket *>(client);

    parse(event.ident, client);

    std::queue<Command *> &commands = client->commands;
    if (connect_socket->getStatus() == REGISTER) {
        registerEvent(event.ident, EVFILT_WRITE, EXECUTE, client);
        return;
    }
    while (commands.size()) {
        _executor.connect(commands.front(), client, _env.password);
        commands.pop();
        if (connect_socket->isAuthenticate()) {
            std::cout << "Register Success!" << std::endl;
            _tmp_garbage.erase(client);
            _executor.updateClient(event.ident, client, REGISTER);
            ResponseHandler::handleConnectResponse(client);
            response(connect_socket->getFd(), connect_socket->send_buf);
            break;
        }
    }
    if (connect_socket->send_buf.length())
        response(connect_socket->getFd(), connect_socket->send_buf);
    if (!commands.empty())
        registerEvent(event.ident, EVFILT_WRITE, EXECUTE, client);
}

void Server::handleExecute(int event_idx) {
    std::cout << "==== Execute ====" << std::endl;

    Event &event = _ev_list[event_idx];
    Client *client =
        static_cast<Client *>(event.udata);  // TODO : connect socket
    ConnectSocket *connect_socket = static_cast<ConnectSocket *>(client);
    std::queue<Command *> &commands = connect_socket->commands;

    while (commands.size()) {
        _executor.execute(commands.front(), client);
        commands.pop();
    }

    const std::set<Client *> &client_list = _executor.getClientList();
    std::set<Client *>::iterator receiver_iter = client_list.begin();
    for (; receiver_iter != client_list.end(); ++receiver_iter) {
        if ((*receiver_iter)->getStatus() == TERMINATE){
            _tmp_garbage.erase(*receiver_iter);
            _garbage.insert(*receiver_iter);
        } else
            registerEvent((*receiver_iter)->getFd(), EVFILT_WRITE, EXECUTE,
                          *receiver_iter);
    }
    _executor.clearClientList();

    if (response(event.ident, connect_socket->send_buf) == 0)
        registerEvent(event.ident, EVFILT_WRITE, D_WRITE, 0);
}

void Server::handleTimer(int event_idx) {
    Event &event = _ev_list[event_idx];
    // find(event.ident) // TODO : timer access freed memory
    Client *client =
        static_cast<Client *>(event.udata);  // TODO : connect socket

    registerEvent(event.ident, EVFILT_TIMER, D_TIMER, 0);
    if (_executor.updateClient(event.ident, client, TIMEOUT) == 0)
        _tmp_garbage.insert(client);
}
void Server::handleTimeout() {
    std::set<Client *>::iterator iter = _tmp_garbage.begin();

    for (; iter != _tmp_garbage.end(); ++iter) {
        _garbage.insert(*iter);
        send((*iter)->getFd(), "Timeout\n", 9, 0);
    }
    iter = _garbage.begin();
    for (; iter != _garbage.end(); ++iter) {
        _tmp_garbage.erase(*iter);
    }
}

void Server::handleClose() {
    std::set<Client *>::iterator iter = _garbage.begin();
    for (; iter != _garbage.end(); ++iter) {
        int connected_fd = (*iter)->getFd();
        std::cout << "New client # " << connected_fd << " gone" << std::endl;
        (*iter)->deleteSocket();
        _executor.deleteClient(*iter);
    }
    _garbage.clear();
}

// TODO : naming
int Server::parse(int fd, Client *client) {
    char buf[BUF_SIZE];
    ssize_t n = 0;

    n = recv(fd, &buf, BUF_SIZE, 0);
    if (n == -1) {
        _garbage.insert(client);
        return -1;
    }
    buf[n] = 0;

    client->recv_buf.append(buf);

    // parse commandlines in connect_sockets
    client->commands = _parser.parse(client);
    return (client->commands.size());
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
