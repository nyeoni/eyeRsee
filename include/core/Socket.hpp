#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <netinet/in.h>
#include <queue>

#include "core/Udata.hpp"

#define BUF_SIZE 512

namespace ft {

class SocketBase {
   protected:
    int _fd;

   public:
    SocketBase();
    explicit SocketBase(int fd);
    SocketBase(const SocketBase &copy);
    virtual ~SocketBase();
    SocketBase &operator=(const SocketBase &ref);

    const int getFd() const;
    virtual void createSocket(const int &info) = 0;
    void deleteSocket();
    void setFd(int fd);
    void setNonBlock();
};

class ListenSocket : public SocketBase {
   public:
    void createSocket(const int &port);
};

class ConnectSocket : public SocketBase {
   public:
    std::string recv_buf;

    // char recv_buf[BUF_SIZE];
    bool auth[3];

   protected:
    //    std::queue<Response> send_queue;

   public:
    ConnectSocket();
    ConnectSocket(const ConnectSocket &copy);
    virtual ~ConnectSocket();
    ConnectSocket &operator=(const ConnectSocket &ref);

    void createSocket(const int &listen_fd);
    bool isAuthenticate();
};

}  // namespace ft

#endif /* SOCKET_HPP */
