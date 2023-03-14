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

// 0000
// 0001
enum e_mode {
    OPER_F,         // 1110  : 0
    OPER_T,         // 0001 :  0b00011,  1
    INVITE_ONLY_F,  // =
    INVITE_ONLY_T,  // = 10 : 2
    TOPIC_PRIV_F,   // = 1 < 5,
    TOPIC_PRIV_T,   // = 1 < 4,
    BAN_F,          // = 1 < 7
    BAN_T           // = 1 < 6,
};
}  // namespace ft

#endif  // TYPE_HPP
