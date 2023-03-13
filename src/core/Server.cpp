#include "core/Server.hpp"

#include <sys/socket.h>

#include <iostream>

#include "core/Response.hpp"

namespace ft {
void Env::parse(int argc, char **argv) {
    double d;
    char *back;
    std::string port_str;

    if (argc != 3) throw std::logic_error("invalid argc error");
    port_str = argv[1];
    if (port_str.find('.') != std::string::npos)
        throw std::logic_error(
            "Error: arguments\n[hint] ./ft_irc <port(1025 ~ 65535)>");
    d = std::strtod(port_str.c_str(), &back);
    if (*back || d < 1025 | d > 65535) {
        throw std::logic_error(
            "Error: arguments\n[hint] ./ft_irc <port(1025 ~ 65535)>");
    }
    port = d;
    // TODO password rule (max_len)
    password = argv[2];
}

/****************************************************/
/*                     Server                       */
/****************************************************/

Server::Server() {
    //_connect_socket = new ConnectSocket[256];
    // TODO max_client 할지 말지 여부
    _socket_list.reserve(ft::Env::max_client);
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
    // TODO : [naming] 실제로 register 되는 시점은 여기가 아님.
    registerEvent(_listen_socket.getFd(), ACCEPT);
    //_change_list.clear();
    //_change_cnt = 0;

    // 2. update (server socket)
    while (true) {
        int n = monitorEvent();
        for (int i = 0; i < n; i++) {
            handleEvent(i);
        }
    }
}

/****************************************************/
/*                   EventHandler                   */
/****************************************************/
/**
 * @brief inner class of Server
 * @note handle kqueue
 */
// Server::EventHandler::EventHandler() : _kq_fd(-1) { _change_list.reserve(32);
// } Server::EventHandler::~EventHandler() {}

// TODO client 어떻게 해제 해줄 지 생각 해야함
void Server::handleAccept() {
    ConnectSocket *connect_socket;

    connect_socket = new ConnectSocket;
    connect_socket->createSocket(_listen_socket.getFd());

    registerEvent(connect_socket->getFd(), READ);
}

void Server::handleRead(int event_idx) {
    const int fd = (int) _ev_list[event_idx].ident;
    char buf[BUFSIZ];
    ssize_t n = 0;

    // buf -> udata
    n = recv(fd, buf, BUFSIZ, 0);
    // if (n < 0)
    //    std::err << "client #" << fd << " wrong read" << std::endl;
    if (n > 0) {
        std::cout << "Receive msg: " << buf << " (fd: " << fd << ")"
                  << std::endl;
        // std::cout << "read " << n << " bytes" << std::endl;
        // if (
        //  TODO : parsing
//        registerEvent(fd, EXCUTE);
    }
}

void Server::handleExcute(int event_idx) {
    // TODO : CommandToIdex(fd);
    // int i;
    // excute[i];

    // ex.PRIVMSG

    std::cout << "execute " << event_idx << std::endl;
    registerEvent(_ev_list[event_idx].ident, DEL_EXCUTE);
    registerEvent(_ev_list[event_idx].ident, WRITE);
    // TODO : findClie`nt(fd).channel.client_list;
    // Command
    // vector<Client> client_list;
    // for (iterator it = client_list.begin(); it != client_list.end(); ++it)
    {
        // registerEvent(it->fd, WRITE);
        //}
    }
}

void Server::handleWrite(int event_idx) {
    // (X) TODO : findSocket(fd).response.buf || findResponse(fd).buf;
    // TODO : udata.buf
    std::cout << "write " << event_idx << std::endl;
    registerEvent(_ev_list[event_idx].ident,
                  DEL_WRITE);  // every client in client_list has their own
    // buf... message must be send in once.... (if
    // particial send occures, message can be mixedF
    // with others) n = send(fd, buf.c_str(),
    // buf.length(), 0); if (n != -1)
    //    register(DEL_WRITE);
}

}  // namespace ft
