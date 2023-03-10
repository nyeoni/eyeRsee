#include "Socket.hpp"

SocketBase::SocketBase() {}

SocketBase::SocketBase(const SocketBase &copy) { *this = copy; }

SocketBase::~SocketBase() {
    if (buf) delete buf;
    close(fd);
}

SocketBase &SocketBase::operator=(const SocketBase &ref) { return (*this); }
