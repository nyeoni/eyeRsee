#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <vector>

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
    int status;
    e_cmd cmd;
    std::string msg;
};

struct Buffer {
    char *buf;
    size_t len;
    size_t current;

    Buffer() : buf(NULL), len(0), current(0) {}
    ~Buffer() {
        if (buf) delete buf;
    }
};

struct Udata_base {
    int action;
};

struct UdataRead : public Udata_base {
    std::vector<char> pre_buf;
    std::vector<std::string> params;
};

struct UdataWrite : public Udata_base {
    Response res;
};

}  // namespace ft

#endif
