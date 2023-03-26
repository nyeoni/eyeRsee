#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

#include <queue>
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
    e_delimiter delimiter;
    std::string recv_buf;
    std::string send_buf;

    bool auth[3];

    std::queue<Command *> commands;

   protected:
    e_status _status;

    std::string _nickname;
    std::string _username;
    std::string _hostname;
    std::string _servername;
    std::string _realname;

   public:
    ConnectSocket();
    ConnectSocket(const ConnectSocket &copy);
    virtual ~ConnectSocket();

    ConnectSocket &operator=(const ConnectSocket &ref);

    void createSocket(const int &listen_fd);

    // getter
    e_status getStatus() const;
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getHostname() const;
    const std::string &getServername() const;
    const std::string &getRealname() const;

    // setter
    void setStatus(e_status status);
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setHostname(const std::string &hostname);
    void setServername(const std::string &servername);
    void setRealname(const std::string &realname);

    bool isAuthenticate();

    std::string readRecvBuf();
};

}  // namespace ft

#endif /* SOCKET_HPP */
