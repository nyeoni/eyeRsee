#include "handler/ErrorHandler.hpp"

#include "core/Parser.hpp"
#include "entity/Client.hpp"

namespace ft {

/**
 * @brief Handle error add error response to send_buf
 * @param client client source
 * @param cause cause of error (command or param)
 * @param code error code
 */
void ErrorHandler::handleError(Client *client, std::string cause,
                               e_err_code code) {
    std::stringstream res_stream;
    std::string res;
    std::string msg = getErrorMessage(code);

    res_stream << ":" << servername << " " << code << " "
               << client->getNickname() << " " << cause << " :\"" << msg
               << "\"";
    res = res_stream.str();
    client->send_buf.append(res);
}
void ErrorHandler::handleError(std::exception &e, Client *src) {
    if (Parser::UnknownCommandException *uce =
            dynamic_cast<Parser::UnknownCommandException *>(&e))
        handleError(src, uce->getCause(), ERR_UNKNOWNCOMMAND);
    else if (Parser::NotEnoughParamsException *nepe =
                 dynamic_cast<Parser::NotEnoughParamsException *>(&e))
        handleError(src, nepe->getCause(), ERR_NEEDMOREPARAMS);
    else if (Parser::InvalidChannelNameException *icne =
                 dynamic_cast<Parser::InvalidChannelNameException *>(&e))
        handleError(src, icne->getCause(), ERR_BADCHANMASK);
    else if (Parser::InvalidNickNameException *inne =
                 dynamic_cast<Parser::InvalidNickNameException *>(&e))
        handleError(src, inne->getCause(), ERR_ERRONEUSNICKNAME);
    else
        std::cout << "ErrorHandler: Unknown error occurred" << std::endl;
}

std::string ErrorHandler::getErrorMessage(e_err_code code) {
    switch (code) {
        case ERR_UNKNOWNMODE:
            return ERR_UNKNOWNCOMMAND_MSG;
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

const std::string ErrorHandler::servername = "eyeRsee.local";

const std::string ErrorHandler::ERR_UNKNOWNCOMMAND_MSG = "Unknown command";
const std::string ErrorHandler::ERR_NEEDMOREPARAMS_MSG =
    "Not enough parameters";

const std::string ErrorHandler::ERR_ALREADYREGISTERED_MSG =
    "You may not reregister";
const std::string ErrorHandler::ERR_NONICKNAMEGIVEN_MSG = "No nickname given";
const std::string ErrorHandler::ERR_ERRONEUSNICKNAME_MSG = "Erroneus nickname";
const std::string ErrorHandler::ERR_NICKNAMEINUSE_MSG =
    "Nickname is already in use";
const std::string ErrorHandler::ERR_NOTONCHANNEL_MSG =
    "You're not on that channel";
const std::string ErrorHandler::ERR_USERONCHANNEL_MSG = "is already on channel";
const std::string ErrorHandler::ERR_UNKNOWNMODE_MSG =
    "is unknown mode char to me";
const std::string ErrorHandler::ERR_INVITEONLYCHAN_MSG =
    "Cannot join channel (+i)";
const std::string ErrorHandler::ERR_CHANOPRIVSNEEDED_MSG =
    "You're not channel operator";
const std::string ErrorHandler::ERR_NOSUCHNICK_MSG = "No such nick/channel";
const std::string ErrorHandler::ERR_NOSUCHCHANNEL_MSG = "No such channel";
const std::string ErrorHandler::ERR_BADCHANMASK_MSG = "Invalid channel name";

}  // namespace ft