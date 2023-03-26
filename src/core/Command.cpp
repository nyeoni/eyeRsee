#include "core/Command.hpp"

namespace ft {

Command::Command() : type(UNKNOWN), params(NULL){};
Command::~Command() {
    if (params) {
        delete params;
    }
};

std::string Command::getCommand() {
    switch (type) {
        case PASS:
            return "PASS";
            break;
        case USER:
            return "USER";
            break;
        case NICK:
            return "NICK";
            break;
        case JOIN:
            return "JOIN";
            break;
        case PART:
            return "PART";
            break;
        case MODE:
            return "MODE";
            break;
        case INVITE:
            return "INVITE";
            break;
        case KICK:
            return "KICK";
            break;
        case TOPIC:
            return "TOPIC";
            break;
        case PRIVMSG:
            return "PRIVMSG";
            break;
        case NOTICE:
            return "NOTICE";
            break;
        case PING:
            return "PING";
            break;
        case BOT:
            return "BOT";
            break;
        default:
            return "";
            break;
    }
}

}  // namespace ft
