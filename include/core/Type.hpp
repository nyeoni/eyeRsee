#ifndef TYPE_HPP
#define TYPE_HPP

namespace ft {
enum e_event { ACCEPT, CONNECT, READ, EXECUTE, WRITE, D_READ, D_WRITE, IDLE };

enum e_filt { FILT_READ, FILT_WRITE };

enum e_cmd {
    PASS,
    USER,
    NICK,
    QUIT,
    JOIN,
    PART,
    MODE,
    INVITE,
    KICK,
    TOPIC,
    PRIVMSG,
    NOTICE,
    PING,
    PONG,
    UNKNOWN
};

enum e_mode {
    OPER_F = 0,     // 11111101
    OPER_T,         // 00000010 -> 1 << 1
    INVITE_ONLY_F,  // 11110111
    INVITE_ONLY_T,  // 00001000 -> 1 << 3
    TOPIC_PRIV_F,   // 11011111
    TOPIC_PRIV_T,   // 00100000 -> 1 << 5
    BAN_F,          // 01111111
    BAN_T           // 10000000 -> 1 << 7
};
}  // namespace ft

#endif  // TYPE_HPP
