#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

namespace ft {

namespace filt {
static const int ReadEvent = 1;
static const int WriteEvent = 2;
}  // namespace filt

enum e_cmd {
    PASS,
    USER,
    NICK,
    // OPER,
    // QUIT,
    JOIN,
    PART,
    MODE,
    INVITE,
    KICK,
    PRIVMSG,
    NOTICE,
    KILL,
    PING,
    PONG
};

struct Response {
    std::string buf;
    size_t len;
    size_t current;
};

template <typename Tp>
struct HandlerInfo {
    int action;
    int listen_fd;
    Tp connect_sockets;

    void init(int fd, Tp sockets) {
        action(0), listen_fd(fd), connect_sockets(sockets);
    }
};

struct Udata_base {
    int action;
};

struct UdataRead : public Udata_base {
    std::string buf;
};

struct UdataWrite : public Udata_base {
    std::string buf;
};

template <typename Tp>
struct UdataAccept : public Udata_base {
    int listen_fd;
    Tp _socket_list;

    void init(int fd, Tp sockets) {
        action(0), listen_fd(fd), _socket_list(sockets);
    }
    void set(int act) { action = act; }
};

// struct Udata {
//  Client
//  channel
//};

}  // namespace ft

#endif
