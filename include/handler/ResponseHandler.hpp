#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <string>

#include "core/Socket.hpp"

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
    static void handleResponse(ConnectSocket *src, const std::string &command,
                               e_res_code res_code);
    static void handleResponse(ConnectSocket *src, const std::string &command,
                               const std::string &param,
                               const std::string &msg = "");
    static void handlePongResponse(ConnectSocket *src);

    // SECTION
    static std::string createResponse(ConnectSocket *src,
                                      const std::string &command,
                                      const std::string &param,
                                      const std::string &msg = "");

    static std::string createResponse(ConnectSocket *src,
                                      const std::string &command,
                                      e_res_code res_code,
                                      const std::string &comment = "");

    static void handleConnectResponse(ConnectSocket *src);

   private:
    static std::string getMessage(e_res_code res_code);
};  // namespace ft

}  // namespace ft

#endif  // RESPONSEHANDLER_HPP
