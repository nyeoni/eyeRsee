#ifndef SOCKET_HPP
#define SOCKET_HPP

namespace ft {

class SocketBase {
   protected:
    int _fd;           // socket fd
    std::string _buf;  // buf of socket_fd received_buf
                       // std::string _hostname;
                       // in_port_t _port

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
