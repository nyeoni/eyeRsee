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

void ResponseHandler::handleResponse(ConnectSocket *src,
                                     const std::string &command,
                                     e_res_code res_code) {
    std::stringstream res_stream;
    std::string res;
    std::string msg = getMessage(res_code);

    res_stream << ":" << servername << " " << res_code << " "
               << src->getNickname() << " " << command << ":\"" << msg << "\""
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
               << src->getHostname() << " " << command;
    if (msg.empty())
        res_stream << " :" << src->getNickname();
    else
        res_stream << " " << param << ":" << msg << std::endl;
    res = res_stream.str();
    src->send_buf.append(res);
}

void ResponseHandler::handleResponse(ConnectSocket *src, ConnectSocket *dst,
                                     const std::string &command,
                                     const std::string &param,
                                     const std::string &msg) {
    std::stringstream res_stream;
    std::string res;

    res_stream << ":" << param << "!" << src->getUsername() << "@"
               << src->getHostname() << " " << command;
    if (msg.empty())
        res_stream << " :" << src->getNickname();
    else
        res_stream << " " << param << ":" << msg << std::endl;
    res = res_stream.str();
    src->send_buf.append(res);
}

std::string ResponseHandler::createResponse(ConnectSocket *src,
                                            const std::string &command,
                                            const std::string &param,
                                            const std::string &msg) {
    std::stringstream res_stream;

    res_stream << ":" << src->getNickname() << "!" << src->getUsername() << "@"
               << src->getHostname() << " " << command;
    if (msg.empty())
        res_stream << " :" << param << std::endl;
    else
        res_stream << " " << param << " :\"" << msg << "\"" << std::endl;
    return res_stream.str();
    // src->send_buf.append(res);
}

std::string ResponseHandler::createResponse(ConnectSocket *src,
                                            const std::string &command,
                                            e_res_code res_code,
                                            const std::string &comment) {
    std::stringstream res_stream;
    std::string msg = getMessage(res_code);
    char sp = 0;

    if (command != "") sp = ' ';
    res_stream.fill('0');
    res_stream << ":" << servername << " " << std::setw(3) << res_code << " "
               << src->getNickname() << " " << command << sp << ":" << msg
               << " " << comment << std::endl;
    return res_stream.str();
    // src->send_buf.append(res);
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
            // case RPL_NAMREPLY:
            //     return;
            // case RPL_ENDOFNAMES:
            //     return;
        default:
            return "Unknown response code";
    }
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

}  // namespace ft
