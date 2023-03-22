#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <iomanip>
#include <sstream>
#include <string>

#include "entity/Client.hpp"

#define WELCOME_PROMPT \
    "\n\
\n\
███████╗██╗   ██╗███████╗██████╗ ███████╗███████╗███████╗\n\
██╔════╝╚██╗ ██╔╝██╔════╝██╔══██╗██╔════╝██╔════╝██╔════╝\n\
█████╗   ╚████╔╝ █████╗  ██████╔╝███████╗█████╗  █████╗\n\
██╔══╝    ╚██╔╝  ██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══╝\n\
███████╗   ██║   ███████╗██║  ██║███████║███████╗███████╗\n\
╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\
\n\
WELCOME TO \033[0;36meyeRsee\033[0m SERVER by KIMx3\n"

namespace ft {
enum e_res_code {
    RPL_WELCOME = 1,
    RPL_YOURHOST = 2,
    RPL_CREATED = 3,
    RPL_MYINFO = 4,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_INVITING = 341,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
};

class ResponseHandler {
    static const std::string servername;

    static const std::string RPL_WELCOME_MSG;
    static const std::string RPL_YOURHOST_MSG;
    static const std::string RPL_CREATED_MSG;
    static const std::string RPL_MYINFO_MSG;

    static const std::string RPL_NOTOPIC_MSG;
    static const std::string RPL_TOPIC_MSG;

   public:
    static void handleResponse(Client *src, Client *dst, std::string command,
                               std::string param, std::string msg = "") {
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
                               std::string param, std::string msg = "") {
        std::stringstream res_stream;
        std::string res;

        res_stream << ":" << client->getNickname() << "!"
                   << client->getUsername() << "@" << client->getHostname()
                   << " " << command;
        if (msg.empty())
            res_stream << " :" << param;
        else
            res_stream << " " << param << ":" << msg << std::endl;
        res = res_stream.str();
        client->send_buf.append(res);
    }
    static void handleResponse(Client *client, std::string command,
                               e_res_code res_code) {
        std::stringstream res_stream;
        std::string res;
        std::string msg = getMessage(res_code);

        res_stream << ":" << servername << " " << res_code << " "
                   << client->getNickname() << " " << command << ":\"" << msg
                   << "\"" << std::endl;
        res = res_stream.str();
        client->send_buf.append(res);
    }
    static void handleConnectResponse(Client *client) {
        const std::string prefix = client->getNickname() + "!" +
                                   client->getUsername() + "@" +
                                   client->getHostname();
        // TODO date
        std::string res_comment[4] = {prefix, "", "",
                                      "eyeRsee.local 1.0 o oit"};
        for (int i = 0; i < 4; i++) {
            std::string res;
            e_res_code res_code = static_cast<e_res_code>(i + 1);
            if (i == 3)
                res = createResponse(client, res_comment[i], res_code, "");
            else
                res = createResponse(client, "", static_cast<e_res_code>(i + 1),
                                     res_comment[i]);
            client->send_buf.append(res);
        }
        client->send_buf.append(WELCOME_PROMPT);
    }
    static std::string createResponse(Client *client, std::string command,
                                      std::string param, std::string msg = "") {
        std::stringstream res_stream;

        res_stream << ":" << client->getNickname() << "!"
                   << client->getUsername() << "@" << client->getHostname()
                   << " " << command;
        if (msg.empty())
            res_stream << " :" << param << std::endl;
        else
            res_stream << " " << param << " :\"" << msg << "\"" << std::endl;
        return res_stream.str();
    }
    static std::string createResponse(Client *client, std::string command,
                                      e_res_code res_code,
                                      std::string comment = "") {
        std::stringstream res_stream;
        std::string msg = getMessage(res_code);

        if (command != "") command += " ";
        res_stream.fill('0');
        res_stream << ":" << servername << " " << std::setw(3) << res_code
                   << " " << client->getNickname() << " " << command << ":"
                   << msg << " " << comment << std::endl;
        return res_stream.str();
    }

   private:
    static std::string getMessage(e_res_code res_code) {
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
};

}  // namespace ft

#endif  // RESPONSEHANDLER_HPP
