#ifndef PARSER_HPP
#define PARSER_HPP

#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "core/Type.hpp"
namespace ft {

class ConnectSocket;
class Command;
class params;

class Parser {
   private:
    enum { TOKEN, MSG };
    std::istringstream tokenStream;
    std::string token;

   protected:
    int getToken(int flag);
    bool isEOF();

    static bool isSpecial(char c);
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

    Command *parse(const std::string &command_line, Command *&command);
    std::queue<Command *> parse(ConnectSocket *src);

    class SyntaxException : public std::exception {
       protected:
        std::string _cause;

       public:
        SyntaxException(const std::string &cause);
        ~SyntaxException() throw();

        std::string getCause();
    };
    class UnknownCommandException : public SyntaxException {
       public:
        UnknownCommandException(const std::string &cause)
            : SyntaxException(cause){};
    };
    class NotEnoughParamsException : public SyntaxException {
       public:
        NotEnoughParamsException(const std::string &cause)
            : SyntaxException(cause){};
    };
    class InvalidChannelNameException : public SyntaxException {
       public:
        InvalidChannelNameException(const std::string &cause)
            : SyntaxException(cause){};
    };
    class InvalidNickNameException : public SyntaxException {
       public:
        InvalidNickNameException(const std::string &cause)
            : SyntaxException(cause){};
    };
    class ModeUserException : public SyntaxException {
       public:
        ModeUserException(const std::string &cause) : SyntaxException(cause){};
    };
};

}  // namespace ft
#endif  // PARSER_HPP
