#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <sstream>
#include <string>

#include "entity/Client.hpp"
#include "handler/EventHandler.hpp"

namespace ft {
enum e_res_code {
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
};

enum e_res_code {
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
};

class ResponseHandler {
    static const std::string servername;

    static const std::string RPL_NOTOPIC_MSG;
    static const std::string RPL_TOPIC_MSG;

   public:
    static void handleResponse(Client *src, Client *dst, std::string command,
                               std::string param, std::string msg = "") {
        //        127.000.000.001.60452-127.000.000.001.06667: PRIVMSG #room
        //        :hihi 127.000.000.001.06667-127.000.000.001.60454:
        //        :b!chloek@127.0.0.1 PRIVMSG #room :hihi
        std::stringstream res_stream;
        std::string res;

        res_stream << ":" << src->getNickname() << "!" << src->getUsername()
                   << "@" << src->getHostname() << " " << command;
        if (msg.empty())
            res_stream << " :" << param;
        else
            res_stream << " " << param << " :\"" << msg << "\"" << std::endl;
        res = res_stream.str();
        dst->send_buf.append(res);
    }
    static void handleResponse(Client *client, std::string command,
                               e_res_code res_code) {
        std::stringstream res_stream;
        std::string res;
        std::string msg = getMessage(res_code);

        res_stream << ":" << servername << " " << std::to_string(res_code)
                   << " " << client->getNickname() << " " << command << " :\""
                   << msg << "\"" << std::endl;
        res = res_stream.str();
        client->send_buf.append(res);
    }

    // SECTION
    static std::string createResponse(Client *client, std::string command,
                                      std::string param, std::string msg = "") {
        std::stringstream res_stream;

        res_stream << ":" << client->getNickname() << "!"
                   << client->getUsername() << "@" << client->getHostname()
                   << " " << command;
        if (msg.empty())
            res_stream << " :" << param;
        else
            res_stream << " " << param << " :\"" << msg << "\"";
        return res_stream.str();
        // client->send_buf.append(res);
    }

    static std::string createResponse(Client *client, std::string command,
                                      e_res_code res_code) {
        std::stringstream res_stream;
        std::string msg = getMessage(res_code);

        res_stream << ":" << servername << " " << std::to_string(res_code)
                   << " " << client->getNickname() << " " << command << " :\""
                   << msg << "\"";
        return res_stream.str();
        // client->send_buf.append(res);
    }

    static std::string getMessage(e_res_code res_code) {
        switch (res_code) {
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
};  // namespace ft

}  // namespace ft

#endif  // RESPONSEHANDLER_HPP
