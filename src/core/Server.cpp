
#include "core/Server.hpp"

#include <sys/socket.h>
#include <unistd.h>  // close

#include <iostream>
#include <sstream>

#include "core/Type.hpp"
#include "core/Udata.hpp"
#include "entity/Client.hpp"

namespace ft {
std::vector<std::string> split(std::string str,
                               char Delimiter);  // 임시로 만든거
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
    ConnectSocket new_socket;
    Client *new_client;
    Udata *data;

    new_socket.createSocket(_listen_socket.getFd());
    new_client = _executor.newClient(new_socket.getFd());
    new_client->setFd(new_socket.getFd());

    // :NAYEON.local 001 nickname :Welcome to the Omega IRC Network
    // nickname!username@127.0.0.1
    data = new Udata;
    data->src = new_client;

    // TODO : timeout error. registerEvent(TIME_OUT) timer 설정해서 등록
    // -> handleTimeout 에서 delete, close
    registerEvent(new_client->getFd(), CONNECT, data);
    // registerEvent(new_client->getFd(), TIMEOUT, data);
}

void Server::handleConnect(int event_idx) {
    std::cout << "Connect" << std::endl;
    std::string command_line;  // command_line
    char buf[BUF_SIZE];
    ssize_t n = 0;
    Event event = _ev_list[event_idx];
    Client *new_client = ((Udata *)event.udata)->src;

    n = recv(event.ident, &buf, BUF_SIZE, 0);

    buf[n] = 0;
    new_client->recv_buf += buf;
    std::string::size_type pos;
    pos = new_client->recv_buf.find('\n');
    if (pos != std::string::npos) {
        //_env.password;
        command_line = new_client->recv_buf.substr(0, pos);
        //_parser.parse(command_line);
        new_client->recv_buf =
            new_client->recv_buf.substr(pos + 1, new_client->recv_buf.length())
                .c_str();
    }
    if (n > 0) {
        std::cout << "Connect msg: " << buf << " (fd: " << event.ident << ")"
                  << std::endl;
        std::cout << "\tcommand_line: " << command_line << std::endl;
        std::cout << "\trecv_buf: " << new_client->recv_buf << std::endl;
    }
    // 임시로 쓰겠습니다
    //_executor.connect(event.ident, static_cast<Udata *>(event.udata));
    std::vector<std::string> cmd_line = split(command_line, ' ');
    _executor.connect(event.ident, static_cast<Udata *>(event.udata), cmd_line);
    if (new_client->isAuthenticate()) {
        registerEvent(event.ident, READ, (Udata *)event.udata);
        std::cout << "#" << event.ident << "READ event registered!"
                  << std::endl;
    }
}

void Server::handleRead(int event_idx) {
    std::cout << "Read" << std::endl;

    char buf[BUF_SIZE];
    Event event = _ev_list[event_idx];                  // event
    ConnectSocket *sock = ((Udata *)event.udata)->src;  // conn_sock
    std::string command_line;                           // command_line
    ssize_t n = 0;                                      // recv_cnt

    n = recv(event.ident, &buf, BUF_SIZE, 0);
    buf[n] = 0;

    std::cout << buf << std::endl;
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
    registerEvent(_ev_list[event_idx].ident, DEL_EXCUTE,
                  (Udata *)_ev_list[event_idx].udata);
    registerEvent(_ev_list[event_idx].ident, WRITE,
                  (Udata *)_ev_list[event_idx].udata);
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
    registerEvent(_ev_list[event_idx].ident, DEL_WRITE,
                  (Udata *)_ev_list[event_idx]
                      .udata);  // every client in client_list has their own
    // buf... message must be send in once.... (if
    // particial send occures, message can be mixedF
    // with others) n = send(fd, buf.c_str(),
    // buf.length(), 0); if (n != -1)
    //    register(DEL_WRITE);
}

}  // namespace ft
