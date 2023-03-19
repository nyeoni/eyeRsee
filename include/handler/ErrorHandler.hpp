#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <string>

namespace ft {

class ErrorHandler {
    enum e_err_code {
        ERR_NEEDMOREPARAMS = 461,
        ERR_ALREADYREGISTERED = 462,
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME = 432,
        ERR_NICKNAMEINUSE = 433,
        ERR_NOTONCHANNEL = 442,
        ERR_USERONCHANNEL = 443,
        ERR_UNKNOWNMODE = 472,
        ERR_INVITEONLYCHAN = 473,
        ERR_CHANOPRIVSNEEDED = 482,
        ERR_NOSUCHNICK = 401,
        ERR_NOSUCHCHANNEL = 403,
        RPL_NOTOPIC = 331,
        RPL_TOPIC = 332
    };
    static const std::string ERR_NEEDMOREPARAMS_MSG;
    static const std::string ERR_ALREADYREGISTERED_MSG;
    static const std::string ERR_NONICKNAMEGIVEN_MSG;
    static const std::string ERR_ERRONEUSNICKNAME_MSG;
    static const std::string ERR_NICKNAMEINUSE_MSG;
    static const std::string ERR_NOTONCHANNEL_MSG;
    static const std::string ERR_USERONCHANNEL_MSG;
    static const std::string ERR_UNKNOWNMODE_MSG;
    static const std::string ERR_INVITEONLYCHAN_MSG;
    static const std::string ERR_CHANOPRIVSNEEDED_MSG;
    static const std::string ERR_NOSUCHNICK_MSG;
    static const std::string ERR_NOSUCHCHANNEL_MSG;
    static const std::string RPL_NOTOPIC_MSG;
    static const std::string RPL_TOPIC_MSG;

   public:
    static std::string getErrorMessage(e_err_code code) {
        switch (code) {
            case ERR_NEEDMOREPARAMS:
                return ERR_NEEDMOREPARAMS_MSG;
            case ERR_ALREADYREGISTERED:
                return ERR_ALREADYREGISTERED_MSG;
            case ERR_NONICKNAMEGIVEN:
                return ERR_NONICKNAMEGIVEN_MSG;
            case ERR_ERRONEUSNICKNAME:
                return ERR_ERRONEUSNICKNAME_MSG;
            case ERR_NICKNAMEINUSE:
                return ERR_NICKNAMEINUSE_MSG;
            case ERR_NOTONCHANNEL:
                return ERR_NOTONCHANNEL_MSG;
            case ERR_USERONCHANNEL:
                return ERR_USERONCHANNEL_MSG;
            case ERR_UNKNOWNMODE:
                return ERR_UNKNOWNMODE_MSG;
            case ERR_INVITEONLYCHAN:
                return ERR_INVITEONLYCHAN_MSG;
            case ERR_CHANOPRIVSNEEDED:
                return ERR_CHANOPRIVSNEEDED_MSG;
            case ERR_NOSUCHNICK:
                return ERR_NOSUCHNICK_MSG;
            case ERR_NOSUCHCHANNEL:
                return ERR_NOSUCHCHANNEL_MSG;
            case RPL_NOTOPIC:
                return RPL_NOTOPIC_MSG;
            case RPL_TOPIC:
                return RPL_TOPIC_MSG;

        }
    }

};

const std::string ErrorHandler::ERR_NEEDMOREPARAMS_MSG = "Not enough parameters";
const std::string ERR_ALREADYREGISTERED_MSG = "You may not reregister";
const std::string ERR_NONICKNAMEGIVEN_MSG = "No nickname given";
const std::string ERR_ERRONEUSNICKNAME_MSG = "Erroneus nickname";
const std::string ERR_NICKNAMEINUSE_MSG = "Nickname is already in use";
const std::string ERR_NOTONCHANNEL_MSG = "You're not on that channel";
const std::string ERR_USERONCHANNEL_MSG = "is already on channel";
const std::string ERR_UNKNOWNMODE_MSG = "is unknown mode char to me";
const std::string ERR_INVITEONLYCHAN_MSG = "Cannot join channel (+i)";
const std::string ERR_CHANOPRIVSNEEDED_MSG = "You're not channel operator";
const std::string ERR_NOSUCHNICK_MSG = "No such nick/channel";
const std::string ERR_NOSUCHCHANNEL_MSG = "No such channel";
const std::string RPL_NOTOPIC_MSG = "No topic is set";
const std::string RPL_TOPIC_MSG = "";

}

#endif //ERRORHANDLER_HPP
