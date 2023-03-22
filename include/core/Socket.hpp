#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

#include <string>

#include "core/Command.hpp"

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

    e_status status;
    bool auth[3];

    std::string _nickname;
    std::string _username;
    std::string _hostname;
    std::string _server;
    std::string _realname;

    std::vector<Command *> commands;

   protected:
   public:
    ConnectSocket();
    ConnectSocket(const ConnectSocket &copy);
    virtual ~ConnectSocket();

    ConnectSocket &operator=(const ConnectSocket &ref);

    void createSocket(const int &listen_fd);

    // getter
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getHostname() const;
    const std::string &getRealname() const;

    // setter
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setHostname(const std::string &hostname);
    void setServer(const std::string &server);
    void setRealname(const std::string &realname);

    bool isAuthenticate();

    std::string readRecvBuf();
};

}  // namespace ft

#endif /* SOCKET_HPP */
