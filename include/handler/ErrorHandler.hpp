#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <string>
#include "entity/Client.hpp"
#include "core/Parser.hpp"

namespace ft {

class ErrorHandler {
    enum e_err_code {
        RPL_NOTOPIC = 331,
        RPL_TOPIC = 332,
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
    void handleError(Parser::UnknownCommandException &e) {
        // NAYEON.local 421 nickname SDFSDF :Unknown command

    }
    void handleError(Parser::NotEnoughParamsException &e) {
        // NAYEON.local 461 nickname JOIN :Not enough parameters.
    }
    void handleError(Parser::InvalidChannelNameException &e) {
        // join aaa
        // NAYEON.local 476 nickname aaa :Invalid channel name
        // aaa : channel name
    }
    void handleError(Parser::InvalidNickNameException &e) {
        // NAYEON.local 432 nickname 123 :Erroneous Nickname
    }
    void handleError(std::exception &e, Client *src) {
//        if (Parser::UnknownCommandException *uce = dynamic_cast<Parser::UnknownCommandException *>(&e)) {
//            handleError(*uce);
//        } else if (Parser::NotEnoughParamsException *nepe = dynamic_cast<Parser::NotEnoughParamsException *>(&e)) {
//            handleError(*nepe);
//        } else if (Parser::InvalidChannelNameException
//            *icne = dynamic_cast<Parser::InvalidChannelNameException *>(&e)) {
//            handleError(*icne);
//        } else if (Parser::InvalidNickNameException *inne = dynamic_cast<Parser::InvalidNickNameException *>(&e)) {
//            handleError(*inne);
//        } else {
//            std::cout << "ErrorHandler: Unknown error occurred" << std::endl;
//        }
    }
    static void handleResponse(Client *client, std::string command, std::string param, std::string msg = "") {
        std::stringstream res_stream;
        std::string res;

        res_stream << ":" << client->getNickname() << "!" << client->getUsername() << "@" << client->getHostname()
                   << " " << command;
        if (msg.empty())
            res_stream << " :" << param;
        else
            res_stream << " " << param << " :\"" << msg << "\"";
        res = res_stream.str();
        client->send_buf.append(res);
    }
    static void handleResponse(Client *client, std::string command, e_err_code code) {
        std::stringstream res_stream;
        std::string res;
        std::string msg = getErrorMessage(code);

        res_stream << ":" << servername << " " << std::to_string(code)
                   << " " << client->getNickname() << " " << command
                   << " :\"" << msg << "\"";
        res = res_stream.str();
        client->send_buf.append(res);
    }

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
            case ERR_BADCHANMASK:
                return ERR_BADCHANMASK_MSG;
            default:
                return "Unknown error code";
        }
    }
};

const std::string ErrorHandler::servername = "eyeRsee.local";

const std::string ErrorHandler::ERR_UNKNOWNCOMMAND_MSG = "Unknown command";
const std::string ErrorHandler::ERR_NEEDMOREPARAMS_MSG = "Not enough parameters";

const std::string ErrorHandler::ERR_ALREADYREGISTERED_MSG = "You may not reregister";
const std::string ErrorHandler::ERR_NONICKNAMEGIVEN_MSG = "No nickname given";
const std::string ErrorHandler::ERR_ERRONEUSNICKNAME_MSG = "Erroneus nickname";
const std::string ErrorHandler::ERR_NICKNAMEINUSE_MSG = "Nickname is already in use";
const std::string ErrorHandler::ERR_NOTONCHANNEL_MSG = "You're not on that channel";
const std::string ErrorHandler::ERR_USERONCHANNEL_MSG = "is already on channel";
const std::string ErrorHandler::ERR_UNKNOWNMODE_MSG = "is unknown mode char to me";
const std::string ErrorHandler::ERR_INVITEONLYCHAN_MSG = "Cannot join channel (+i)";
const std::string ErrorHandler::ERR_CHANOPRIVSNEEDED_MSG = "You're not channel operator";
const std::string ErrorHandler::ERR_NOSUCHNICK_MSG = "No such nick/channel";
const std::string ErrorHandler::ERR_NOSUCHCHANNEL_MSG = "No such channel";
const std::string ErrorHandler::ERR_BADCHANMASK_MSG = "Invalid channel name";

}

#endif //ERRORHANDLER_HPP
