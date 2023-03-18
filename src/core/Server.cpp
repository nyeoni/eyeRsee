
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
long long getTicks(void);                        // utility.cpp

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
    Client *new_client = ((Udata *)event.udata)->src;
    Udata *udata = static_cast<Udata *>(event.udata);

    n = recv(event.ident, &buf, BUF_SIZE, 0);
    buf[n] = 0;
    new_client->recv_buf += buf;
    std::string::size_type pos;

    pos = new_client->recv_buf.find('\n');
    if (pos != std::string::npos) {
        command_line = new_client->recv_buf.substr(0, pos);
        _parser.parse(command_line, ((Udata *)udata)->command,
                      ((Udata *)udata)->params);
        new_client->recv_buf =
            new_client->recv_buf.substr(pos + 1, new_client->recv_buf.length());
    }
    if (n > 0) {
        std::cout << "Connect msg: " << buf << " (fd: " << event.ident << ")"
                  << std::endl;
        std::cout << "\tcommand_line: " << command_line << std::endl;
        std::cout << "\trecv_buf: " << new_client->recv_buf << std::endl;
    }
    switch (udata->command) {
        case PASS:
            _executor.pass(new_client,
                           dynamic_cast<pass_params *>(udata->params)->password,
                           _env.password);
            break;
        case USER:
            _executor.user(
                new_client,
                dynamic_cast<user_params *>(udata->params)->username,
                dynamic_cast<user_params *>(udata->params)->hostname,
                dynamic_cast<user_params *>(udata->params)->servername,
                dynamic_cast<user_params *>(udata->params)->realname);
            break;
        case NICK:
            _executor.nick(
                new_client,
                dynamic_cast<nick_params *>(udata->params)->nickname);
            break;
        default:
            // TODO :NAYEON.local 451 * JOIN :You have not registered.
            break;
    }
    if (new_client->isAuthenticate()) {
        registerEvent(event.ident, READ, (Udata *)event.udata);
        registerEvent(event.ident, WRITE, (Udata *)event.udata);
        std::cout << "#" << event.ident << "READ event registered!"
                  << std::endl;
    }
}

void Server::handleRead(int event_idx) {
    std::cout << "==== Read ====" << std::endl;

    char buf[BUF_SIZE];
    Event event = _ev_list[event_idx];
    ssize_t n = 0;

    ConnectSocket *sock = ((Udata *)event.udata)->src;

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
        _parser.parse(command_line, ((Udata *)event.udata)->command,
                      ((Udata *)event.udata)->params);

        sock->recv_buf = str_buf.substr(pos + 1, str_buf.length());
    }

    //    std::vector<std::string> tokens = split(buf, ' ');
}

void Server::handleExecute(int event_idx) {
    std::cout << "execute " << event_idx << std::endl;

    Udata *udata = static_cast<Udata *>(_ev_list[event_idx].udata);
    Client *client = udata->src;
    int numeric_replie;
    int fd = _ev_list[event_idx].ident;

    switch (udata->command) {
        case NICK:
            _executor.nick(client, "nickname");  // TODO nickname
            break;
        case JOIN:
            _executor.join(
                fd, dynamic_cast<join_params *>(udata->params)->channels);
            break;
        case PART:
            _executor.part(
                fd, dynamic_cast<part_params *>(udata->params)->channels);
            break;
        case MODE:
            _executor.mode(fd,
                           dynamic_cast<mode_params *>(udata->params)->channel,
                           dynamic_cast<mode_params *>(udata->params)->mode);
            break;
        case INVITE:
            _executor.invite(fd, "nickname", "channel");  // TODO
            break;
        case KICK:
            _executor.kick(fd, "channel", "nickname", "comment");  // TODO
            break;
        case PRIVMSG:
            _executor.privmsg(
                client,
                dynamic_cast<privmsg_params *>(udata->params)->receivers,
                dynamic_cast<privmsg_params *>(udata->params)->msg);
            break;
            // case NOTICE:
            //      _executor.notice(fd);
            //     break;
            // case PING:
            //      _executor.ping(fd);
            //     break;
            // case PONG:
            //      _executor.pong(fd);
            //     break;
        default:
            break;
    }
    registerEvent(_ev_list[event_idx].ident, WRITE, udata);
}

void Server::handleWrite(int event_idx) {
    // Udata *udata = static_cast<Udata *>(_ev_list[event_idx].udata);
    std::string &send_buf =
        static_cast<Udata *>(_ev_list[event_idx].udata)->src->send_buf;
    std::cout << "write " << event_idx << std::endl;
    ssize_t n;
    n = send(_ev_list[event_idx].ident, send_buf.c_str(), send_buf.length(), 0);
    if (n == send_buf.length())
        registerEvent(_ev_list[event_idx].ident, DEL_WRITE, NULL);
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
