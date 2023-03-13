#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <netinet/in.h>

namespace ft {

class SocketBase {
 protected:
    int _fd;
    struct in_addr sin_addr;

 public:
    SocketBase();
    virtual ~SocketBase();

    virtual void createSocket(const int info) = 0;

    const int &getFd() const;
    const std::string &getBuf() const;

 protected:
    void setNonBlock();
};

class ListenSocket : public SocketBase {
 public:
    void createSocket(const int port);
};

class ConnectSocket : public SocketBase {
 public:
    void createSocket(const int listen_fd);
};

}  // namespace ft

#endif /* SOCKET_HPP */
