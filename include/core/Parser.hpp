#ifndef PARSER_HPP
#define PARSER_HPP

#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include "core/Type.hpp"
namespace ft {

class ConnectSocket;
struct Command;
struct params;

class Parser {
   private:
    enum { TOKEN, MSG };
    std::istringstream tokenStream;
    std::string token;

   protected:
    int getToken(int flag);
    bool isEOF();

   public:
    static bool isSpecial(char c);
    static bool validPassword(std::string &password);
    static bool validChannelName(std::string &channel);
    static std::string validChannelName(std::vector<std::string> &channels);
    static bool validNickName(std::string &nickname);

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
    bool parseBot(e_cmd &cmd, params *&params, std::string &receiver);

    Command *parse(const std::string &command_line, Command *&command);

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
    class UnHandledModeException : public SyntaxException {
       public:
        UnHandledModeException(const std::string &cause)
            : SyntaxException(cause){};
    };
};

}  // namespace ft
#endif  // PARSER_HPP
