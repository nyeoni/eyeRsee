#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

#include <string>

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

    int getFd() const;
    virtual void createSocket(const int &info) = 0;
    void deleteSocket();
    void setFd(int fd);
    void setNonBlock();
};

class ListenSocket : public SocketBase {
   public:
    void createSocket(const int &port);
};

enum e_delimiter { CRLF, LF };
class ConnectSocket : public SocketBase {
   public:
    e_status status;
    e_delimiter delimiter;
    std::string recv_buf;
    std::string send_buf;
    bool auth[3];

   protected:
   public:
    ConnectSocket();
    ConnectSocket(const ConnectSocket &copy);
    virtual ~ConnectSocket();

    ConnectSocket &operator=(const ConnectSocket &ref);

    bool isAuthenticate();
    void createSocket(const int &listen_fd);
    std::string readRecvBuf();
};

}  // namespace ft

#endif /* SOCKET_HPP */
