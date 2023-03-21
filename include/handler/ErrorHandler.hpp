#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <string>

namespace ft {

class Client;

class ErrorHandler {
    enum e_err_code {
        ERR_NOSUCHNICK = 401,
        ERR_NOSUCHCHANNEL = 403,
        ERR_UNKNOWNCOMMAND = 421,
        ERR_NONICKNAMEGIVEN = 431,
        ERR_ERRONEUSNICKNAME = 432,
        ERR_NICKNAMEINUSE = 433,
        ERR_NOTONCHANNEL = 442,
        ERR_USERONCHANNEL = 443,
        ERR_NEEDMOREPARAMS = 461,
        ERR_ALREADYREGISTERED = 462,
        ERR_UNKNOWNMODE = 472,
        ERR_INVITEONLYCHAN = 473,
        ERR_BADCHANMASK = 476,
        ERR_CHANOPRIVSNEEDED = 482,
    };
    static const std::string servername;

    static const std::string ERR_UNKNOWNCOMMAND_MSG;
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
    static const std::string ERR_BADCHANMASK_MSG;

   public:
    static void handleError(Client *client, std::string cause, e_err_code code);
    static void handleError(std::exception &e, Client *src);
    static std::string getErrorMessage(e_err_code code);
};

}

#endif //ERRORHANDLER_HPP
