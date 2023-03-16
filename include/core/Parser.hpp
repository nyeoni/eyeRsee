#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "Type.hpp"
#include "Udata.hpp"

namespace ft {

std::vector<std::string> split(std::string str, char delimiter);
std::vector<std::string> split(std::istringstream &stream, char delimiter);

class Parser {
   private:
    enum { TOKEN, MSG };
    std::istringstream tokenStream;
    std::string token;
   protected:
    int getToken(int flag);
    bool isEOF();

   public:
    void parseQuit(e_cmd &cmd, params *&params);
    void parsePass(e_cmd &cmd, params *&params);
    void parseUser(e_cmd &cmd, params *&params);
    void parseNick(e_cmd &cmd, params *&params);
    void parseJoin(e_cmd &cmd, params *&params);
    void parsePart(e_cmd &cmd, params *&params);
    void parseMode(e_cmd &cmd, params *&params) {}
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
