
#include "core/Server.hpp"

#include <sys/socket.h>
#include <unistd.h>  // close

#include <iostream>
#include <sstream>

#include "core/Type.hpp"
#include "core/Udata.hpp"
#include "core/utility.hpp"
#include "entity/Client.hpp"

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
    new_client = _executor.creatClient(new_socket.getFd());
    new_client->setFd(new_socket.getFd());

    data = new Udata;
    data->src = new_client;

    registerEvent(new_client->getFd(), CONNECT, data);
    // registerEvent(new_client->getFd(), TIMEOUT, data); // TODO
}

void Server::handleConnect(int event_idx) {
    std::cout << "Connect" << std::endl;
    std::string command_line;
    char buf[BUF_SIZE];
    ssize_t n = 0;
    Event event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);
    Client *new_client = udata->src;

    n = recv(event.ident, &buf, BUF_SIZE, 0);
    buf[n] = 0;
    new_client->recv_buf += buf;
    std::string::size_type pos;

    pos = new_client->recv_buf.find('\n');
    if (pos != std::string::npos) {
        command_line = new_client->recv_buf.substr(0, pos);
        // _parser.parse(command_line, ((Udata *)udata)->command,
        //               ((Udata *)udata)->params);
        new_client->recv_buf =
            new_client->recv_buf.substr(pos + 1, new_client->recv_buf.length());
    }
    if (n > 0) {
        std::cout << "Connect msg: " << buf << " (fd: " << event.ident << ")"
                  << std::endl;
        std::cout << "\tcommand_line: " << command_line << std::endl;
        std::cout << "\trecv_buf: " << new_client->recv_buf << std::endl;
    }

    std::vector<Command>::iterator iter = udata->commands.begin();
    for (; iter != udata->commands.end(); ++iter) {
        _executor.connect(&*iter, new_client, _env.password);
    }

    if (new_client->isAuthenticate()) {
        send(event.ident, WELCOME_PROMPT, strlen(WELCOME_PROMPT), 0);

        registerEvent(event.ident, READ, (Udata *)event.udata);
        registerEvent(event.ident, DEL_WRITE,
                      static_cast<Udata *>(event.udata));
        std::cout << "#" << event.ident << "READ event registered!"
                  << std::endl;
    }
}

void Server::handleRead(int event_idx) {
    std::cout << "==== Read ====" << std::endl;

    char buf[BUF_SIZE];
    Event event = _ev_list[event_idx];
    ssize_t n = 0;
    Udata *udata = static_cast<Udata *>(event.udata);
    ConnectSocket *sock = udata->src;

    n = recv(event.ident, &buf, BUF_SIZE, 0);
    buf[n] = 0;
    sock->recv_buf += buf;

    std::cout << "rev_buf: " << sock->recv_buf << std::endl;

    // get command_line from buf
    std::string str_buf(buf, n);
    std::string command_line;
    std::string::size_type pos = str_buf.find('\n');

    if (pos != std::string::npos) {
        command_line = str_buf.substr(0, pos);
        // _parser.parse(command_line, ((Udata *)event.udata)->command,
        //               ((Udata *)event.udata)->params);

        sock->recv_buf = str_buf.substr(pos + 1, str_buf.length());

        // if (buf res) register(READ_MORE);
    }

    //    std::vector<std::string> tokens = split(buf, ' ');
}

void Server::handleExecute(int event_idx) {
    std::cout << "execute " << event_idx << std::endl;

    Udata *udata = static_cast<Udata *>(_ev_list[event_idx].udata);
    Client *client = udata->src;
    int numeric_replie;
    int fd = _ev_list[event_idx].ident;

    std::vector<Command>::iterator iter = udata->commands.begin();
    for (; iter != udata->commands.end(); ++iter) {
        _executor.execute(&*iter, client);
    }

    registerEvent(_ev_list[event_idx].ident, WRITE, udata);
}

void Server::handleWrite(int event_idx) {
    Event &event = _ev_list[event_idx];
    std::string &send_buf = static_cast<Udata *>(event.udata)->src->send_buf;

    std::cout << "write " << event_idx << std::endl;
    ssize_t n;
    n = send(event.ident, send_buf.c_str(), send_buf.length(), 0);
    if (n == send_buf.length())
        registerEvent(event.ident, DEL_WRITE, NULL);
    else if (n == -1) {
        std::cerr << "[UB] send return -1" << std::endl;
    } else {
        send_buf = send_buf.substr(n, send_buf.length());
    }
}

void Server::handleTimeout(int event_idx) {
    long long start =
        static_cast<Udata *>(_ev_list[event_idx].udata)->src->create_time;
    // registerEvent(_ev_list[event_idx].ident, DEL_WRITE, 0);  // 나중에고치기

    if (getTicks() > start + 10000) {
        registerEvent(_ev_list[event_idx].ident, DEL_READ, 0);
        registerEvent(_ev_list[event_idx].ident, CLOSE, 0);
    }
}

const char *Parser::SyntaxException::what() const throw() {
    return exception::what();
}
}  // namespace ft
