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

const int SocketBase::getFd() const { return _fd; }

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
    // 임시
    while (bind(_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        sin.sin_port = htons(port + ++i);
    }
    std::cout << "listening port : " << port + i << std::endl;

    // if (bind(_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    //     throw std::logic_error("bind error");                         //
    //     CHECK
    if (listen(_fd, 42) < 0) throw std::logic_error("listen error");  // CHECK

    // setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);
    setNonBlock();
}

/****************************************************/
/*                  ConnectSocket                   */
/****************************************************/

ConnectSocket::ConnectSocket() : SocketBase(-1) {
    create_time = getTicks();
    recv_buf.reserve(512);
    send_buf.reserve(512);
}
ConnectSocket::ConnectSocket(const ConnectSocket &copy)
    : SocketBase(copy.getFd()) {}
ConnectSocket::~ConnectSocket() {}
ConnectSocket &ConnectSocket::operator=(const ConnectSocket &ref) {
    return (*this);
}

void ConnectSocket::createSocket(const int &listen_fd) {
    struct sockaddr_in in = {};
    socklen_t in_len = sizeof(in);

    _fd = accept(listen_fd, (struct sockaddr *)&in, &in_len);
    setNonBlock();
    // TODO : password
    // recv(connect_fd, buf, 0, 0); // TODO : why warning in irssi (CR/LF)
    std::cout << "New client # " << _fd << " from " << inet_ntoa(in.sin_addr)
              << ':' << ntohs(in.sin_port) << std::endl;
}

bool ConnectSocket::isAuthenticate() { return auth[0] && auth[1] && auth[2]; }

std::string ConnectSocket::readRecvBuf() {
    std::string line;
    std::string::size_type found = recv_buf.find_last_of('\n');

    line = recv_buf.substr(0, found);
    recv_buf = recv_buf.substr(found + 1);
    return (line);
}

}  // namespace ft
