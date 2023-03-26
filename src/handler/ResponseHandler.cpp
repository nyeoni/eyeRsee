#include "handler/ResponseHandler.hpp"

#include <iomanip>
#include <sstream>

namespace ft {

const std::string ResponseHandler::servername = "eyeRsee.local";

const std::string ResponseHandler::RPL_WELCOME_MSG =
    "Welcome to the Internet Relay Network";
const std::string ResponseHandler::RPL_YOURHOST_MSG =
    "Your host is eyeRsee.local, running version 1.0";
const std::string ResponseHandler::RPL_CREATED_MSG =
    "This server was created 15:58:02 Mar 20 2023";
const std::string ResponseHandler::RPL_MYINFO_MSG = "";

const std::string ResponseHandler::RPL_NOTOPIC_MSG = "No topic is set";
const std::string ResponseHandler::RPL_TOPIC_MSG = "";

const std::string ResponseHandler::RPL_ENDOFNAMES_MSG = "End of /NAMES list";

void ResponseHandler::handleResponse(ConnectSocket *src,
                                     const std::string &command,
                                     e_res_code res_code) {
    std::stringstream res_stream;
    std::string res;
    std::string msg = getMessage(res_code);

    res_stream << ":" << servername << " " << res_code << " "
               << src->getNickname() << " " << command << ":" << msg
               << std::endl;
    res = res_stream.str();
    src->send_buf.append(res);
}

void ResponseHandler::handleResponse(ConnectSocket *src,
                                     const std::string &command,
                                     const std::string &param,
                                     const std::string &msg) {
    std::stringstream res_stream;
    std::string res;

    res_stream << ":" << src->getNickname() << "!" << src->getUsername() << "@"
               << src->getServername() << " " << command;
    if (msg.empty())
        res_stream << " :" << param << std::endl;
    else
        res_stream << " " << param << " :" << msg << std::endl;
    res = res_stream.str();
    src->send_buf.append(res);
}

std::string ResponseHandler::createResponse(ConnectSocket *src,
                                            const std::string &command,
                                            const std::string &param,
                                            const std::string &msg) {
    std::stringstream res_stream;

    res_stream << ":" << src->getNickname() << "!" << src->getUsername() << "@"
               << src->getServername() << " " << command;
    if (msg.empty())
        res_stream << " :" << param << std::endl;
    else
        res_stream << " " << param << " :" << msg << std::endl;
    return res_stream.str();
}

std::string ResponseHandler::createResponse(ConnectSocket *src,
                                            const std::string &command,
                                            e_res_code res_code,
                                            const std::string &comment) {
    std::stringstream res_stream;
    std::string cmd = command;
    std::string msg = getMessage(res_code);

    if (command != "") cmd += " ";
    res_stream.fill('0');
    res_stream << ":" << servername << " " << std::setw(3) << res_code << " "
               << src->getNickname() << " " << cmd << ":" << msg << " "
               << comment << std::endl;
    return res_stream.str();
}

std::string ResponseHandler::createJoinReponse(
    const std::string &channel_name, const std::string &nickname,
    std::vector<std::string> &operators, std::vector<std::string> &regulars) {
    std::stringstream res_stream;
    std::vector<std::string>::iterator iter;

    res_stream.fill('0');
    // 353
    res_stream << ":" << servername << " " << RPL_NAMREPLY << " " << nickname
               << " = " << channel_name << " :";

    // regulars
    for (iter = regulars.begin(); iter != regulars.end(); ++iter) {
        res_stream << *iter << " ";
    }
    // operators
    for (iter = operators.begin(); iter != operators.end(); ++iter) {
        res_stream << "@" << *iter << " ";
    }
    res_stream << std::endl;

    // 366
    res_stream << ":" << servername << " " << RPL_ENDOFNAMES << " " << nickname
               << " " << channel_name << " " << getMessage(RPL_ENDOFNAMES)
               << std::endl;

    return res_stream.str();
}

void ResponseHandler::handleInviteResponse(ConnectSocket *invitor,
                                           ConnectSocket *target,
                                           const std::string &channel_name) {
    std::stringstream invitor_res;
    std::stringstream target_res;

    invitor_res.fill('0');
    target_res.fill('0');
    // 341
    invitor_res << invitor->getServername() << " " << RPL_INVITING << " "
                << invitor->getNickname() << " " << target->getNickname()
                << " :" << channel_name << std::endl;
    invitor->send_buf.append(invitor_res.str());

    target_res << ":" << invitor->getNickname() << "!" << invitor->getUsername()
               << "@" << invitor->getServername() << " INVITE "
               << target->getNickname() << " :" << channel_name << std::endl;
    target->send_buf.append(target_res.str());
}

void ResponseHandler::handleConnectResponse(ConnectSocket *src) {
    const std::string prefix = src->getNickname() + "!" + src->getUsername() +
                               "@" + src->getHostname();
    // TODO date
    std::string res_comment[4] = {prefix, "", "", "eyeRsee.local 1.0 o oit"};
    for (int i = 0; i < 4; i++) {
        std::string res;
        e_res_code res_code = static_cast<e_res_code>(i + 1);
        if (i == 3)
            res = createResponse(src, res_comment[i], res_code, "");
        else
            res = createResponse(src, "", static_cast<e_res_code>(i + 1),
                                 res_comment[i]);
        src->send_buf.append(res);
    }
    src->send_buf.append(WELCOME_PROMPT);
}

void ResponseHandler::handlePongResponse(ConnectSocket *src) {
    const std::string res =
        ":" + servername + " PONG " + servername + " :" + servername + "\n";
    src->send_buf.append(res);
}

void ResponseHandler::handleBotResponse(ConnectSocket *src, std::string &param,
                                        std::string &msg) {
    const std::string prefix =
        ":bot!" + src->getUsername() + "@" + src->getHostname();
    const std::string res = prefix + " PRIVMSG " + param + " :" + msg + "\n";
    src->send_buf.append(res);
}

std::string ResponseHandler::getMessage(e_res_code res_code) {
    switch (res_code) {
        case RPL_WELCOME:
            return RPL_WELCOME_MSG;
        case RPL_YOURHOST:
            return RPL_YOURHOST_MSG;
        case RPL_CREATED:
            return RPL_CREATED_MSG;
        case RPL_MYINFO:
            return RPL_MYINFO_MSG;
        case RPL_NOTOPIC:
            return RPL_NOTOPIC_MSG;
        case RPL_TOPIC:
            return RPL_TOPIC_MSG;
        case RPL_ENDOFNAMES:
            return RPL_ENDOFNAMES_MSG;
        default:
            return "Unknown response code";
    }
}

}  // namespace ft
