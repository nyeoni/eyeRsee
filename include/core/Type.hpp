#ifndef TYPE_HPP
#define TYPE_HPP

namespace ft {
enum e_event {
    ACCEPT,
    CONNECT,
    READ,
    READ_MORE,
    WRITE,
    WRITE_MORE,
    EXCUTE,
    DEL_READ,
    DEL_EXCUTE,
    DEL_WRITE,
    IDLE
};

enum e_cmd {
    PASS,
    USER,
    NICK,
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

enum e_mode {
    OPER_T = 1,
    OPER_F = 1 < 1,
    INVITE_ONLY_T = 1 < 2,
    INVITE_ONLY_F = 1 < 3,
    TOPIC_PRIV_T = 1 < 4,
    TOPIC_PRIV_F = 1 < 5,
    BAN_T = 1 < 6,
    BAN_F = 1 < 7
};
}

#endif //TYPE_HPP
