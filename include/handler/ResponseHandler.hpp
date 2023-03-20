#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <string>

#include "entity/Client.hpp"

namespace ft {

class ResponseHandler {
    enum e_res_code {
        RPL_NOTOPIC = 331,
        RPL_TOPIC = 332,
    };
    static const std::string servername;

    static const std::string RPL_NOTOPIC_MSG;
    static const std::string RPL_TOPIC_MSG;

   public:
    static void handleResponse(Client *client, std::string command,
                               std::string param, std::string msg = "") {
        std::stringstream res_stream;
        std::string res;

        res_stream << ":" << client->getNickname() << "!"
                   << client->getUsername() << "@" << client->getHostname()
                   << " " << command;
        if (msg.empty())
            res_stream << " :" << param;
        else
            res_stream << " " << param << " :\"" << msg << "\"";
        res = res_stream.str();
        client->send_buf.append(res);
    }
    static void handleResponse(Client *client, std::string command,
                               e_res_code res_code) {
        std::stringstream res_stream;
        std::string res;
        std::string msg = getErrorMessage(res_code);

        res_stream << ":" << servername << " " << std::to_string(res_code)
                   << " " << client->getNickname() << " " << command << " :\""
                   << msg << "\"";
        res = res_stream.str();
        client->send_buf.append(res);
    }

    static std::string getErrorMessage(e_res_code res_code) {
        switch (res_code) {
            case RPL_NOTOPIC:
                return RPL_NOTOPIC_MSG;
            case RPL_TOPIC:
                return RPL_TOPIC_MSG;
            default:
                return "Unknown response code";
        }
    }
};

const std::string ResponseHandler::servername = "eyeRsee.local";

const std::string ResponseHandler::RPL_NOTOPIC_MSG = "No topic is set";
const std::string ResponseHandler::RPL_TOPIC_MSG = "";

}  // namespace ft

#endif  // RESPONSEHANDLER_HPP
