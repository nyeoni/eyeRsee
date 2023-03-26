#ifndef TYPE_HPP
#define TYPE_HPP

namespace ft {
enum e_event { ACCEPT, READ, EXECUTE, TIMER, D_WRITE, D_TIMER, IDLE };

enum e_status { UNREGISTER, REGISTER, TIMEOUT, TERMINATE };

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
    BOT,
    UNKNOWN
};

enum e_mode {
    OPER_F = 0,
    OPER_T,
    INVITE_ONLY_F,
    INVITE_ONLY_T,
    TOPIC_PRIV_F,
    TOPIC_PRIV_T,
    BAN_F,
    BAN_T
};

enum e_role { OPERATOR = 0, REGULAR };

}  // namespace ft

#endif  // TYPE_HPP
