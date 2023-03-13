#include "core/Server.hpp"
#include "core/Udata.hpp"
#include "core/Type.hpp"
#include "entity/Client.hpp"
#include <sys/socket.h>
#include <iostream>

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
    // TODO : [naming] 실제로 register 되는 시점은 여기가 아님.
    registerEvent(_listen_socket.getFd(), ACCEPT, 0);
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

void Server::handleAccept() {
    std::cout << "Accept" << std::endl;
    Client *new_client;
    Udata *data;

    new_client = new Client;

    new_client->createSocket(_listen_socket.getFd());

    // :NAYEON.local 001 nickname :Welcome to the Omega IRC Network nickname!username@127.0.0.1
    data = new Udata;
    data->src = new_client;

    registerEvent(new_client->getFd(), CONNECT, data);
}

void Server::handleConnect(int event_idx) {
    std::cout << "Connect" << std::endl;
    ssize_t n = 0;
    Event event = _ev_list[event_idx];
    Client *new_client = ((Udata *) event.udata)->src;

    // pass, nick, user 순으로 명령어가 들어와야 함 -> 아닐 시 timeout error
    // nick 이랑 user 가 세트로 같이 오는지 확인 아니면 error
    // nick / user polling 으로 계속 기다리기

//    int i = 0;
//    unsigned int flag = 0;
//    while (flag != 0b11) {
//        std::string command_line;
//
//        n = recv(event.ident, new_client->recv_buf, BUF_SIZE, 0);
//        std::string str_buf(new_client->recv_buf, n);
//
//        if (str_buf.find('\n') != std::string::npos) {
////            command_line = "";
//        }
//        flag = 0b11;
//        i++;
//    }

    _executer.connect(new_client);
    registerEvent(new_client->getFd(), READ, (Udata *) event.udata);
}

void Server::handleRead(int event_idx) {
    std::cout << "Read" << std::endl;

    Event event = _ev_list[event_idx]; // event
    ConnectSocket *sock = ((Udata *) event.udata)->src; // conn_sock
    std::string command_line; // command_line
    ssize_t n = 0; // recv_cnt

    n = recv(event.ident, sock->recv_buf, BUF_SIZE, 0);

    std::cout << sock->recv_buf << std::endl;
//    std::string str_buf(sock->recv_buf[0], n);


//    std::string::size_type pos = str_buf.find('\n');
//    if (pos != std::string::npos) {
//        command_line = str_buf.substr(0, pos);
//        _parser.parse(command_line);
//        std::string tmp = sock->recv_buf;
//        sock->recv_buf = str_buf.substr(pos, str_buf.length()).c_str();
//    }
//    if (n > 0) {
//        std::cout << "Receive msg: " << buf << " (fd: " << fd << ")"
//                  << std::endl;
//         if (
//          TODO : parsing
//        registerEvent(fd, EXCUTE);
//    }
}

void Server::handleExecute(int event_idx) {
    std::cout << "execute " << event_idx << std::endl;
    registerEvent(_ev_list[event_idx].ident,
                  DEL_EXCUTE,
                  (Udata *) _ev_list[event_idx].udata);
    registerEvent(_ev_list[event_idx].ident,
                  WRITE,
                  (Udata *) _ev_list[event_idx].udata);
    // TODO : findClie`nt(fd).channel.client_list;
    // Command
    // vector<Client> client_list;
    // for (iterator it = client_list.begin(); it != client_list.end(); ++it)
    {
        // registerEvent(it->fd, WRITE);
    }
}

void Server::handleWrite(int event_idx) {
    // (X) TODO : findSocket(fd).response.buf || findResponse(fd).buf;
    // TODO : udata.buf
    std::cout << "write " << event_idx << std::endl;
    registerEvent(_ev_list[event_idx].ident,
                  DEL_WRITE,
                  (Udata *) _ev_list[event_idx].udata);  // every client in client_list has their own
    // buf... message must be send in once.... (if
    // particial send occures, message can be mixedF
    // with others) n = send(fd, buf.c_str(),
    // buf.length(), 0); if (n != -1)
    //    register(DEL_WRITE);
}

}  // namespace ft
