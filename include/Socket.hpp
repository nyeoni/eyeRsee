#ifndef SOCKET_HPP
#define SOCKET_HPP

namespace ft {

class SocketBase {
   private:
    int fd;     // socket fd
    char *buf;  // buf of socket_fd received_buf
   public:
    SocketBase(/* args*/);
    virtual ~SocketBase();
    SocketBase &operator=(const SocketBase &ref);

    virtual void createSocket() = 0;
};

class ListeningSocket : public SocketBase {};

class ConnectSocket : public SocketBase {};

#endif

}  // namespace ft

#endif /* SOCKET_HPP */