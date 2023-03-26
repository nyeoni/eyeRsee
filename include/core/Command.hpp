#ifndef UDATA_HPP
#define UDATA_HPP

#include <string>
#include <vector>

#include "Type.hpp"

namespace ft {

class Client;

enum e_bot_cmd {
    BOT_HELP,
    BOT_NOW,
    BOT_HI,
    BOT_BYE,
    BOT_WEATHER,
    BOT_NKIM,
    BOT_HANNKIM,
    BOT_CHAEJKIM,
    BOT_EYERSEE
};

struct params {
    virtual ~params() {}
};
struct pass_params : public params {
    std::string password;
};
struct user_params : public params {
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
};
struct nick_params : public params {
    std::string nickname;
};
struct quit_params : public params {
    std::string msg;
};
struct join_params : public params {
    std::vector<std::string> channels;
    std::vector<std::string> keys;
};
struct part_params : public params {
    std::vector<std::string> channels;
};
struct mode_params : public params {
    std::string channel;
    e_mode mode;
    std::string nickname;
};
struct invite_params : public params {
    std::string nickname;
    std::string channel;
};
/**
 * @brief comment is optional
 */
struct kick_params : public params {
    std::string channel;
    std::string user;
    std::string comment;
};
struct topic_params : public params {
    std::string channel;
    std::string topic;
};
struct bot_params : public params {
    e_bot_cmd cmd;
    std::string receiver;
};
struct privmsg_params : public params {
    std::vector<std::string> receivers;
    std::string msg;
};
struct notice_params : public params {
    std::string nickname;
    std::string msg;
};
struct ping_params : public params {
    std::string servername;
};

struct Command {
    e_cmd type;
    params *params;

    Command();
    ~Command();

    std::string getCommand();
};

}  // namespace ft

#endif
