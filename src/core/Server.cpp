
#include "core/Server.hpp"

#include <sys/socket.h>
#include <unistd.h>

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
    if (*back || d_port<1025 | d_port> 65535) {
        throw std::logic_error(
            "Error: arguments\n[hint] ./ft_irc <port(1025 ~ 65535)>");
    }
    port = static_cast<int>(d_port);
    password = argv[2];
    if (!Parser::validPassword(password)) {
        throw std::logic_error(
            "Error: invalid password\n[hint] <length 3~9, ascii, and "
            "non-isspace>");
    }
}

/****************************************************/
/*                     Server                       */
/****************************************************/

Server::Server() {}
Server::~Server() {}

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
    Client *new_client;
    int connected_fd;

    new_client = _executor.accept(_listen_socket.getFd());

    connected_fd = new_client->getFd();

    registerEvent(connected_fd, EVFILT_READ, READ, new_client);
    registerEvent(connected_fd, EVFILT_TIMER, TIMER, new_client);
}

void Server::handleRead(int event_idx) {
    Event event = _ev_list[event_idx];
    Client *client = static_cast<Client *>(event.udata);
    ConnectSocket *connect_socket = static_cast<ConnectSocket *>(client);
    e_status status = connect_socket->getStatus();

    parse(event.ident, client);

    switch (status) {
        case REGISTER:
            registerEvent(event.ident, EVFILT_WRITE, EXECUTE, client);
            break;
        case UNREGISTER:
            connect(event.ident, client);
            break;
        default:
            destroyCommands(client->commands);
            break;
    }
}

void Server::handleExecute(int event_idx) {
    Event &event = _ev_list[event_idx];
    Client *client = static_cast<Client *>(event.udata);
    ConnectSocket *connect_socket = static_cast<ConnectSocket *>(client);
    std::queue<Command *> &commands = connect_socket->commands;

    if (commands.size()) {
        _executor.execute(commands.front(), client);
        commands.pop();
    }

    reserve();

    if (response(event.ident, connect_socket->send_buf) == 0 &&
        commands.size() == 0)
        registerEvent(event.ident, EVFILT_WRITE, D_WRITE, 0);
}

void Server::handleTimer(int event_idx) {
    Event &event = _ev_list[event_idx];
    Client *client = static_cast<Client *>(event.udata);

    registerEvent(event.ident, EVFILT_TIMER, D_TIMER, 0);
    if (_executor.updateClientStatus(event.ident, client, TIMEOUT) == 0) {
        _garbage.insert(client);
        send(client->getFd(), "Timeout\n", 9, 0);
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

int Server::parse(int fd, Client *src) {
    char buf[BUF_SIZE];
    ssize_t n = 0;

    std::string line;
    std::queue<Command *> &commands = src->commands;
    std::vector<std::string> command_lines;

    n = recv(fd, &buf, BUF_SIZE, 0);
    if (n == -1) {
        _garbage.insert(src);
        return -1;
    }
    buf[n] = 0;

    src->recv_buf.append(buf);

    // parse commandlines in connect_sockets
    line = src->readRecvBuf();

    if (src->delimiter == CRLF)
        command_lines = split(line, "\r\n");
    else
        command_lines = split(line, '\n');

    std::vector<std::string>::iterator it;
    for (it = command_lines.begin(); it != command_lines.end(); it++) {
        Command *command = new Command;
        try {
            _parser.parse(*it, command);
            if (command != NULL) commands.push(command);
        } catch (Parser::SyntaxException &e) {
            delete command;
            ErrorHandler::handleError(e, src);
        }
    }
    return (src->commands.size());
}

void Server::connect(int fd, Client *client) {
    std::queue<Command *> &commands = client->commands;

    if (_executor.connect(client, _env.password) == true) {
        std::cout << "Register Success!" << std::endl;
        ResponseHandler::handleConnectResponse(client);
        response(fd, client->send_buf);
    } else if (!commands.empty()) {
        registerEvent(fd, EVFILT_WRITE, EXECUTE, client);
    }
    if (client->send_buf.length()) {
        response(client->getFd(), client->send_buf);
    }
}

void Server::reserve() {
    const std::set<Client *> &client_list = _executor.getClientList();
    std::set<Client *>::iterator receiver_iter = client_list.begin();
    for (; receiver_iter != client_list.end(); ++receiver_iter) {
        if ((*receiver_iter)->getStatus() == TERMINATE)
            _garbage.insert(*receiver_iter);
        else
            registerEvent((*receiver_iter)->getFd(), EVFILT_WRITE, EXECUTE,
                          *receiver_iter);
    }
    _executor.clearClientList();
}

int Server::response(int fd, std::string &send_buf) {
    ssize_t n;
    n = send(fd, send_buf.c_str(), send_buf.length(), 0);
    if (n == static_cast<ssize_t>(send_buf.length())) {
        send_buf.clear();
        return 0;
    }
    if (n > 0)
        send_buf = send_buf.substr(n, send_buf.length());
    else
        std::cerr << "[UB] send return -1" << std::endl;
    return -1;
}

void Server::destroyCommands(std::queue<Command *> &commands) {
    while (commands.size()) {
        delete commands.front();
        commands.pop();
    }
}

}  // namespace ft
