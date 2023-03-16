#include "core/Parser.hpp"

namespace ft {

int Parser::getToken(int flag = TOKEN) {
    if (tokenStream.eof()) return EOF;
    if (flag == MSG)
        std::getline(tokenStream, token);
    else
        std::getline(tokenStream, token, ' ');
    return (1);
}
bool Parser::isEOF() {
    if (tokenStream.eof()) return true;
    return false;

}

void Parser::parseQuit(e_cmd &cmd, params *&params) {
    cmd = QUIT;
    quit_params *p;
    if (getToken(MSG) != EOF) {
        p = new quit_params;
        p->msg = token;
    }
    params = p;
}
void Parser::parsePass(e_cmd &cmd, params *&params) {
    cmd = PASS;
    pass_params *p;

    if (!isEOF() && getToken()) {
        p = new pass_params;
        p->password = token;
    } else {
        throw std::logic_error("Pass argument error");
    }
    params = p;
}
void Parser::parseUser(e_cmd &cmd, params *&params) {
    cmd = USER;
    user_params *p;
    std::vector<std::string> tokens = split(tokenStream, ' ');

    if (tokens.size() != 4) throw std::logic_error("Argument must be 4");
    p = new user_params;
    p->username = tokens[0];
    p->hostname = tokens[1];
    p->servername = tokens[2];
    p->realname = tokens[3];
    params = p;
}
void Parser::parseNick(e_cmd &cmd, params *&params) {
    cmd = NICK;
    nick_params *p;

    std::vector<std::string> tokens = split(tokenStream, ' ');
    if (tokens.size() != 1) throw std::logic_error("Nick parse error");
    p = new nick_params;
    p->nickname = tokens[0];
    params = p;
}
void Parser::parseJoin(e_cmd &cmd, params *&params) {
    cmd = JOIN;
    join_params *p;
    // irssi는 params 1개만 보고 나머지 무시..
    // validate channel name
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
}
void Parser::parsePart(e_cmd &cmd, params *&params) {
    cmd = PART;
    part_params *p;
    if (!isEOF() && getToken()) {
        p = new part_params;
        std::vector<std::string> channels = split(token, ',');
        p->channels = channels;
    } else {
        throw std::logic_error("Arugument must be given");
    }
    params = p;
}

void Parser::parseInvite(e_cmd &cmd, params *&params) {
    invite_params *p;
    cmd = INVITE;
    std::vector<std::string> tokens = split(tokenStream, ' ');

    if (tokens.size() != 2) throw std::logic_error("Argument must be 2");
    p->nickname = tokens[0];
    p->channel = tokens[1];
    params = p;
}
void Parser::parseKick(e_cmd &cmd, params *&params) {
    cmd = KICK;
    kick_params *p;

    if (!isEOF() && getToken()) {
        p = new kick_params;
        p->channel = token;
        if (!isEOF() && getToken()) {
            p->user = token;
            if (getToken(MSG) != EOF) {
                p->comment = token;
            }
        } else {
            delete p;
            throw std::logic_error("Argument must be 2 or 3");
        }
    } else {
        throw std::logic_error("Argument must be 2 or 3");
    }
    params = p;
}
void Parser::parseTopic(e_cmd &cmd, params *&params) {
    cmd = TOPIC;
    topic_params *p;

    std::vector<std::string> tokens = split(tokenStream, ' ');
    if (!(tokens.size() == 1 || tokens.size() == 2)) {
        throw std::logic_error("Argument must be 2 or 3");
    }
    p = new topic_params;
    p->channel = tokens[0];
    if (tokens.size() == 2) {
        p->topic = tokens[1];
    }
    params = p;
}
void Parser::parsePrivmsg(e_cmd &cmd, params *&params) {
    cmd = PRIVMSG;
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
}
void Parser::parseNotice(e_cmd &cmd, params *&params) {
    cmd = NOTICE;
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
}
void Parser::parsePing(e_cmd &cmd, params *&params) {
    cmd = PING;
    ping_params *p;

    if (!isEOF() && getToken()) {
        p->servername = token;
    } else {
        throw std::logic_error("Ping parsing error");
    }
    params = p;
}

void Parser::parse(const std::string &command_line, e_cmd &cmd, params *&params) {
    tokenStream.str(command_line);
    getToken();

    if (token == "QUIT") {
        parseQuit(cmd, params);
    } else if (token == "PASS") {
        parsePass(cmd, params);
    } else if (token == "USER") {
        parseUser(cmd, params);
    } else if (token == "NICK") {
        parseNick(cmd, params);
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

} // namespace ft
