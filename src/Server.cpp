#include "Server.hpp"

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

Server::Server() {}
Server::~Server() {}

void Server::init(int argc, char **argv) { _env.parse(argc, argv); }

// socket
void Server::run() {
    EventHandler event_handler;
    // 1. server socket
    _listen_socket.createSocket(_env.port);  // env.port
                                             // updateChangeList
    // 2. update (server socket)
    while (true) {
        int n = event_handler.monitorEvent();
        for (int i = 0; i < n; i++) {
            event_handler.handle(i);
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

void Server::EventHandler::updateChangeList(int fd) {}

int Server::EventHandler::monitorEvent() {}

}  // namespace ft
