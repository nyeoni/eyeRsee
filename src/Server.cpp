#include "Server.hpp"

#include <sys/socket.h>

#include <iostream>

#include "Response.hpp"

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
    if (*back || d<1025 | d> 65535) {
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
    _socket_list.reserve(_env.max_client);
}
Server::~Server() {
    // if (_connect_socket) delete[] _connect_socket;
}

void Server::init(int argc, char **argv) { _env.parse(argc, argv); }

// socket
void Server::run() {
    // 1. server socket
    _listen_socket.createSocket(_env.port);  // env.port
    // TODO : [naming] 실제로 register 되는 시점은 여기가 아님.
    registerEvent(_listen_socket.getFd(), ACCEPT);
    // event_handler.updateChangeList(); -> register!

    // 2. update (server socket)
    while (true) {
        int n = monitorEvent();
        for (int i = 0; i < n; i++) {
            handle(i);
        }
    }
    // 3. n = detectEvent();

    // while (true) {
    //     int n = _ev_handler.updateEvent();
    //     for (int i = 0; i < n; i++) {
    //         _ev_handler.handle(i);
    //     }
    // }
}

/****************************************************/
/*                   EventHandler                   */
/****************************************************/
/**
 * @brief inner class of Server
 * @note handle kqueue
 */
Server::EventHandler::EventHandler() : _kq_fd(-1) { _change_list.reserve(32); }
Server::EventHandler::~EventHandler() {}

Server::EventHandler::Event *Server::EventHandler::changeList() {
    return static_cast<Event *>(&_change_list[0]);
}

int Server::EventHandler::monitorEvent() {
    int n =
        kevent(_kq_fd, changeList(), _change_cnt, _ev_list, _max_event, NULL);
    _change_cnt = 0;
    // std::cout << "epoll_wait return " << n << std::endl;
    return n;
}

void Server::handleAccept() {
    ConnectSocket tmp;

    // Inner class
    // tmp.createSocket(_listen_socket.getFd());
    //_socket_list.push_back(tmp);

    tmp.createSocket(_listen_socket.getFd());

    _socket_list.push_back(tmp);
    registEvent(tmp.getFd(), READ);
}

void Server::EventHandler::handleRead(int event_idx) {
    std::vector<char> buf;
    buf.reserve(BUFSIZ);  // TODO : buf max size
    int n = 0;

    n = recv(_ev_list[event_idx].ident, &buf[0], BUFSIZ, 0);
    // if (n < 0)
    //   std::err << "client #" << fd << " wrong read" << std::endl;
    if (n > 0) {
        // std::cout << "read " << n << " bytes" << std::endl;
        //  TODO : parsing
        registEvent(_ev_list[event_idx].ident, EXCUTE);
    }
}

// void Server::EventHandler::handleExcute(int fd) {
//     // TODO : CommandToIdex(fd);
//     // int i;
//     // excute[i];

//     // ex.PRIVMSG
//     // TODO : findClient(fd).channel.client_list;
//     // Command
//     // vector<Client> client_list;
//     // for (iterator it = client_list.begin(); it != client_list.end(); ++it)
//     {
//     // registerEvent(it->fd, Write);
//     //}
// }

// void Server::EventHandler::handleWrite(UdataWrite &udata) {
//     // (X) TODO : findSocket(fd).response.buf || findResponse(fd).buf;
//     // TODO : udata.buf
//     // every client in client_list has their own buf...
//     // message must be send in once....
//     // (if particial send occures, message can be mixed with others)
//     // n = send(fd, buf.c_str(), buf.length(), 0);
//     // if (n != -1)
//     //    register(DEL_WRITE);
// }

}  // namespace ft
