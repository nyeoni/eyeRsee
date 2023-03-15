#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

namespace ft {
std::vector<std::string> split(std::string str, char Delimiter);

class Parser {
   public:
    // static std::string cmd[];
    // static int cmd_num;  // 할까 말까
    typedef std::vector<std::string> CmdLine;

    // void parseAuth(Udata *udata, std::string basic_string_1,
    //                std::string password = std::string()) {
    //     CmdLine cmd_line = split(basic_string_1, ' ');
    //     int i = 0;
    //     for (; i < NICK; i++) {
    //         if (cmd[i] == cmd_line[0]) {
    //             break;
    //         }
    //     }
    //     switch (i) {
    //         case PASS:
    //             // Parsing 에서 확인하고 pass(bool)
    //             // if (cmd_line.size() != 2) response 생성
    //             if (cmd_line.size() == 2 && password == cmd[1])
    //                 udata->params = new pass_params(true);
    //             else
    //                 udata->params = new pass_params(false);
    //                 // response 생성 여기서 할지, 서버에서 할지
    //             break;
    //         case USER:
    //             // user(new_client, cmd_line[1], cmd_line[2], cmd_line[3],
    //             // cmd_line[4]);
    //             if (cmd_line.size() == 2 && password == cmd[1]) break;
    //         case NICK:
    //             // nick(new_client, cmd_line[1]);
    //             break;
    //         default:
    //             break;
    //     }
    // }

    void parse(std::string basic_string_1);
    // void parse(std::string basic_string_1) {
    //     CmdLine cmd_line = split(basic_string_1, ' ');
    //     int i = QUIT;
    //     for (; i < cmd_num; i++) {
    //         if (cmd[i] == cmd_line[0]) {
    //             break;
    //         }
    //     }
    //     switch (i) {
    //         case QUIT:
    //             break;
    //         case USER:
    //             break;
    //         case JOIN:
    //             break;
    //         case PART:
    //             break;
    //         case MODE:
    //             break;
    //         case INVITE:
    //             break;
    //         case KICK:
    //             break;
    //         case PRIVMSG:
    //             break;
    //         case NOTICE:
    //             break;
    //         case PING:
    //             break;
    //         case PONG:
    //             break;
    //         default:
    //             break;
    //     }
    // }
};

// std::string Parser::cmd[] = {"PASS",   "USER", "NICK",   "QUIT", "JOIN",
//  "PART",   "MODE", "INVITE", "KICK", "PRIVMSG",
//  "NOTICE", "KILL", "PING",   "PONG"};
// int Parser::cmd_num = sizeof(cmd) / sizeof(std::string);

}  // namespace ft
#endif  // PARSER_HPP
