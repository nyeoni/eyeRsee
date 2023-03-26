#include "core/Socket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>
#include <unistd.h>  // cunistd

#include <iostream>
#include <stdexcept>

#include "core/utility.hpp"

namespace ft {

/****************************************************/
/*                  SocketBase                      */
/****************************************************/

SocketBase::SocketBase() : _fd(-1) {}
SocketBase::SocketBase(int fd) : _fd(fd) {}
SocketBase::~SocketBase() {}
SocketBase::SocketBase(const SocketBase &copy) : _fd(copy.getFd()) {}
SocketBase &SocketBase::operator=(const SocketBase &ref) {
    _fd = ref.getFd();
    return (*this);
}

int SocketBase::getFd() const { return _fd; }

void SocketBase::deleteSocket() {
    // std::cout << "delet?";
    if (_fd != -1) close(_fd);
}
void SocketBase::setFd(int fd) { _fd = fd; }

void SocketBase::setNonBlock() {
    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
        throw std::logic_error("NonBlock Set Error");
}

/****************************************************/
/*                  ListenSocket                    */
/****************************************************/

void ListenSocket::createSocket(const int &port) {
    struct sockaddr_in sin = {};

    _fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_fd < 0) throw std::logic_error("create listen fd error");

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    sin.sin_port = htons(port);

    int i = 0;
    const char spinner[] = "||//--\\\\";
    while (bind(_fd, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        std::cout << "Loading... " << spinner[i % 8] << "\r";
        std::cout.flush();  // flush output buffer
        i++;
        usleep(100000);
    }
    if (listen(_fd, 42) < 0) throw std::logic_error("listen error");
    setNonBlock();
}

/****************************************************/
/*                  ConnectSocket                   */
/****************************************************/

ConnectSocket::ConnectSocket() : SocketBase(-1), _status(UNREGISTER) {
    recv_buf.reserve(512);
    send_buf.reserve(512);
    auth[PASS] = false;
    auth[USER] = false;
    auth[NICK] = false;
}
ConnectSocket::ConnectSocket(const ConnectSocket &copy)
    : SocketBase(copy.getFd()) {}
ConnectSocket::~ConnectSocket() {}
ConnectSocket &ConnectSocket::operator=(const ConnectSocket &ref) {
    this->delimiter = ref.delimiter;
    this->recv_buf = ref.recv_buf;
    this->send_buf = ref.send_buf;

    this->auth[0] = ref.auth[0];
    this->auth[1] = ref.auth[1];
    this->auth[2] = ref.auth[2];
    this->commands = ref.commands;

    this->_status = ref._status;

    this->_nickname = ref._nickname;
    this->_username = ref._username;
    this->_hostname = ref._hostname;
    this->_servername = ref._servername;
    this->_realname = ref._realname;
    return (*this);
}

void ConnectSocket::createSocket(const int &listen_fd) {
    struct sockaddr_in in = {};
    socklen_t in_len = sizeof(in);

    _fd = accept(listen_fd, (struct sockaddr *) &in, &in_len);
    _status = UNREGISTER;
    auth[PASS] = false;
    auth[USER] = false;
    auth[NICK] = false;
    setNonBlock();
    std::cout << "New client # " << _fd << " from " << inet_ntoa(in.sin_addr)
              << ':' << ntohs(in.sin_port) << std::endl;
}

// getter
e_status ConnectSocket::getStatus() const { return _status; }
const std::string &ConnectSocket::getNickname() const { return _nickname; }
const std::string &ConnectSocket::getUsername() const { return _username; }
const std::string &ConnectSocket::getHostname() const { return _hostname; }
const std::string &ConnectSocket::getServername() const { return _servername; }
const std::string &ConnectSocket::getRealname() const { return _realname; }

// setter
void ConnectSocket::setStatus(e_status status) { _status = status; }

void ConnectSocket::setNickname(const std::string &nickname) {
    _nickname = nickname;
}
void ConnectSocket::setUsername(const std::string &username) {
    _username = username;
}
void ConnectSocket::setHostname(const std::string &hostname) {
    _hostname = hostname;
}
void ConnectSocket::setServername(const std::string &server) {
    _servername = server;
}
void ConnectSocket::setRealname(const std::string &realname) {
    _realname = realname;
}

bool ConnectSocket::isAuthenticate() {
    return auth[PASS] && auth[USER] && auth[NICK];
}

std::string ConnectSocket::readRecvBuf() {
    std::string line;
    std::string::size_type found;

    found = recv_buf.rfind("\r\n");
    if (found != std::string::npos) {
        delimiter = CRLF;

        line = recv_buf.substr(0, found);
        recv_buf = recv_buf.substr(found + 2);
    } else {
        found = recv_buf.rfind('\n');
        if (found != std::string::npos) {
            delimiter = LF;

            line = recv_buf.substr(0, found);
            recv_buf = recv_buf.substr(found + 1);
        }
    }
    return (line);
}

}  // namespace ft
