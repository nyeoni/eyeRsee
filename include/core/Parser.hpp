#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core/Type.hpp"
#include "core/Udata.hpp"
#include "core/utility.hpp"
namespace ft {

class Parser {
   private:
    enum { TOKEN, MSG };
    std::istringstream tokenStream;
    std::string token;

   protected:
    int getToken(int flag);
    bool isEOF();

    static bool validSpecial(char c);
    static std::string &validChannelName(std::string &channel);
    static std::vector<std::string> &validChannelName(
        std::vector<std::string> &channels);
    static std::string &validNickName(std::string &nickname);

   public:
    void parseQuit(e_cmd &cmd, params *&params);
    void parsePass(e_cmd &cmd, params *&params);
    void parseUser(e_cmd &cmd, params *&params);
    void parseNick(e_cmd &cmd, params *&params);
    void parseJoin(e_cmd &cmd, params *&params);
    void parsePart(e_cmd &cmd, params *&params);
    void parseMode(e_cmd &cmd, params *&params);
    void parseInvite(e_cmd &cmd, params *&params);
    void parseKick(e_cmd &cmd, params *&params);
    void parseTopic(e_cmd &cmd, params *&params);
    void parsePrivmsg(e_cmd &cmd, params *&params);
    void parseNotice(e_cmd &cmd, params *&params);
    void parsePing(e_cmd &cmd, params *&params);

    void parse(const std::string &command_line, e_cmd &cmd, params *&params);

    class SyntaxException : public std::exception {
       public:
        const char *what() const throw();
    };
};

}  // namespace ft
#endif  // PARSER_HPP
