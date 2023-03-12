#include "Socket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>
#include <unistd.h>  // cunistd

#include <iostream>
#include <stdexcept>

namespace ft {

SocketBase::SocketBase() : _fd(-1) {}

SocketBase::~SocketBase() {
    // if (_buf) delete _buf;
    if (_fd != -1) close(_fd);
}

const int &SocketBase::getFd() const { return _fd; }
const std::string &SocketBase::getBuf() const { return _buf; }

void SocketBase::setNonBlock() {
    if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
        throw std::logic_error("NonBlock Set Error");
}

void ListenSocket::createSocket(const int port) {
    struct sockaddr_in sin;

    _fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_fd < 0) throw std::logic_error("create listen fd error");

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    if (bind(_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        throw std::logic_error("bind error");                         // CHECK
    if (listen(_fd, 42) < 0) throw std::logic_error("listen error");  // CHECK

    setNonBlock();
}

void ConnectSocket::createSocket(const int listen_fd) {
    // char buf[1];
    struct sockaddr_in csin;
    socklen_t csin_len = sizeof(csin);
    _fd = accept(listen_fd, (struct sockaddr *)&csin, &csin_len);
    // TODO : password
    // std::cout << "New client # " << _fd << " from " <<
    // inet_ntoa(csin.sin_addr)
    //   << ':' << ntohs(csin.sin_port) << std::endl;
    setNonBlock();
    // recv(connect_fd, buf, 0, 0); // TODO : why warning in irssi (CR/LF)
    std::cout << "New client # " << _fd << " from " << inet_ntoa(csin.sin_addr)
              << ':' << ntohs(csin.sin_port) << std::endl;
}

}  // namespace ft
