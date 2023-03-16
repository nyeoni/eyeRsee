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
    int getToken(int flag = TOKEN) {
        if (tokenStream.eof()) return EOF;
        if (flag == MSG)
            std::getline(tokenStream, token);
        else
            std::getline(tokenStream, token, ' ');
        return (1);
    }
    bool isEOF() {
        if (tokenStream.eof()) return true;
        return false;
    }

   public:
    void parseQuit(e_cmd &cmd, params *&params) {
        quit_params *p;
        if (getToken(MSG) != EOF) {
            p = new quit_params;
            p->msg = token;
        }
        params = p;
        cmd = QUIT;
    }
    void parsePass(e_cmd &cmd, params *&params) {
        pass_params *p;

        if (!isEOF() && getToken()) {
            p = new pass_params;
            p->password = token;
        } else {
            throw std::logic_error("Pass argument error");
        }
        params = p;
        cmd = PASS;
    }
    void parseUser(e_cmd &cmd, params *&params) {
        user_params *p;
        std::vector<std::string> tokens = split(tokenStream, ' ');

        if (tokens.size() != 4) throw std::logic_error("Argument must be 4");
        p = new user_params;
        p->username = tokens[0];
        p->hostname = tokens[1];
        p->servername = tokens[2];
        p->realname = tokens[3];
        params = p;
        cmd = USER;
    }
    void parseJoin(e_cmd &cmd, params *&params) {
        join_params *p;
        if (!isEOF() && getToken()) {
            p = new join_params;
            std::vector<std::string> channels = split(token, ',');
            p->channels = channels;
            if (!isEOF() && getToken()) {
                std::vector<std::string> keys = split(token, ',');
                p->keys = keys;
            }
        } else {
            throw std::logic_error("Arugument must be given");
        }
        params = p;
        cmd = JOIN;
    }
    void parsePart(e_cmd &cmd, params *&params) {
        part_params *p;
        if (!isEOF() && getToken()) {
            p = new part_params;
            std::vector<std::string> channels = split(token, ',');
            p->channels = channels;
        } else {
            throw std::logic_error("Arugument must be given");
        }
        params = p;
        cmd = PART;
    }
    void parseMode(e_cmd &cmd, params *&params) {

    }
    void parseInvite(e_cmd &cmd, params *&params) {
        invite_params *p;
        std::vector<std::string> tokens = split(tokenStream, ' ');

        if (tokens.size() != 2) throw std::logic_error("Argument must be 2");
        p->nickname = tokens[0];
        p->channel = tokens[1];
        params = p;
        cmd = INVITE;
    }
    void parseKick(e_cmd &cmd, params *&params) {
        kick_params *p;

        if (!isEOF() && getToken()) {
            p = new kick_params;
            p->channel = token;
            if (!isEOF() && getToken()) {
                p->user = token;
                if (getToken(MSG) != EOF)
                    p->comment = token;
            } else {
                delete p;
                throw std::logic_error("Argument must be 2 or 3");
            }
        } else {
            throw std::logic_error("Argument must be 2 or 3");
        }
        params = p;
        cmd = KICK;
    }
    void parseTopic(e_cmd &cmd, params *&params) {
        topic_params *p;

        std::vector<std::string> tokens = split(tokenStream, ' ');
        if (!(tokens.size() == 1 || tokens.size() == 2))
            throw std::logic_error("Argument must be 2 or 3");
        p = new topic_params;
        p->channel = tokens[0];
        if (tokens.size() == 2)
            p->topic = tokens[1];
        params = p;
        cmd = TOPIC;
    }
    void parsePrivmsg(e_cmd &cmd, params *&params) {
        privmsg_params *p;

        if (!isEOF() && getToken()) {
            p = new privmsg_params;
            std::vector<std::string> receivers = split(token, ',');
            p->receivers = receivers;
            if (!isEOF() && getToken(MSG)) {
                p->msg = token;
            } else {
                delete p;
                throw std::logic_error("PRIV parsing error");
            }
        } else {
            throw std::logic_error("Privmsg: Argument error");
        }
        params = p;
        cmd = PRIVMSG;
    }
    void parseNotice(e_cmd &cmd, params *&params) {
        notice_params *p;

        if (!isEOF() && getToken()) {
            p->nickname = token;
            if (!isEOF() && getToken(MSG)) {
                p->msg = token;
            } else {
                delete p;
                throw std::logic_error("Notice argument error");
            }
        } else {
            throw std::logic_error("Notice argument error");
        }
        params = p;
        cmd = NOTICE;
    }
    void parsePing(e_cmd &cmd, params *&params) {
        ping_params *p;

        if (!isEOF() && getToken()) {
            p->servername = token;
        } else {
            throw std::logic_error("Ping parsing error");
        }
        params = p;
        cmd = PING;
    }

    void parse(const std::string &command_line, e_cmd &cmd, params *&params) {
        tokenStream.str(command_line);
        getToken();

        if (token == "QUIT") {
            parseQuit(cmd, params);
        } else if (token == "PASS") {
            parsePass(cmd, params);
        } else if (token == "USER") {
            parseUser(cmd, params);
        } else if (token == "JOIN") {
            parseJoin(cmd, params);
        } else if (token == "PART") {
            parsePart(cmd, params);
        } else if (token == "MODE") {
            parseMode(cmd, params);
        } else if (token == "INVITE") {
            parseInvite(cmd, params);
        } else if (token == "KICK") {
            parseKick(cmd, params);
        } else if (token == "TOPIC") {
            parseTopic(cmd, params);
        } else if (token == "PRIVMSG") {
            parsePrivmsg(cmd, params);
        } else if (token == "NOTICE") {
            parseNotice(cmd, params);
        } else if (token == "PING") {
            parsePing(cmd, params);
        } else {
            std::cout << "wtf: " << token << std::endl;
            throw std::logic_error("Invalid Command");
        }
        tokenStream.clear();
    }

    class SyntaxException : public std::exception {
       public:
        const char *what() const throw();
    };
};

// std::string Parser::cmd[] = {"PASS",   "USER", "NICK",   "QUIT", "JOIN",
//  "PART",   "MODE", "INVITE", "KICK", "PRIVMSG",
//  "NOTICE", "KILL", "PING",   "PONG"};
// int Parser::cmd_num = sizeof(cmd) / sizeof(std::string);

}  // namespace ft
#endif  // PARSER_HPP
