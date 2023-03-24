#include "core/Parser.hpp"

#include "core/Command.hpp"
#include "core/Socket.hpp"
#include "core/utility.hpp"
#include "handler/ErrorHandler.hpp"

namespace ft {

int Parser::getToken(int flag = TOKEN) {
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

bool Parser::isSpecial(char c) {
    if (c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '^' ||
        c == '{' || c == '}' || c == '_')
        return true;
    return false;
}
std::string &Parser::validChannelName(std::string &channel) {
    // chstring any string except for SPACE, BELL, NUL, CR, LF and comma(',')
    if (channel.size() > 200) throw InvalidChannelNameException(channel);
    if (channel[0] != '#') throw InvalidChannelNameException(channel);
    for (int i = 1; i < channel.length(); i++) {
        if (!isascii(channel[i]) || isspace(channel[i]) || channel[i] == ',')
            throw InvalidChannelNameException(channel);
    }
    return channel;
}
std::vector<std::string> &Parser::validChannelName(
    std::vector<std::string> &channels) {
    std::vector<std::string>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++) {
        validChannelName(*it);
    }
    return channels;
}
std::string &Parser::validNickName(std::string &nickname) {
    // <letter> { <letter> | <number> | <special> }
    if (nickname.length() > 9) throw InvalidNickNameException(nickname);
    if (!isalpha(nickname[0])) throw InvalidNickNameException(nickname);
    for (int i = 1; i < nickname.length(); i++) {
        if (!isalnum(nickname[i]) && !isSpecial(nickname[i]))
            throw InvalidNickNameException(nickname);
    }
    return nickname;
}

void Parser::parseQuit(e_cmd &cmd, params *&params) {
    cmd = QUIT;
    quit_params *p;
    p = new quit_params;
    if (getToken(MSG) && token[0] == ':') {
        p->msg = "\"" + token.substr(1) + "\"";
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
        throw NotEnoughParamsException("PASS");
    }
    params = p;
}
void Parser::parseUser(e_cmd &cmd, params *&params) {
    cmd = USER;
    user_params *p;
    std::vector<std::string> tokens = split(tokenStream, ' ');

    if (tokens.size() != 4 && tokens.size() != 5)
        throw NotEnoughParamsException("USER");
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
    if (tokens.size() != 1) throw NotEnoughParamsException("NICK");
    p = new nick_params;
    p->nickname = validNickName(tokens[0]);
    params = p;
}
void Parser::parseJoin(e_cmd &cmd, params *&params) {
    cmd = JOIN;
    join_params *p;

    if (!isEOF() && getToken()) {
        p = new join_params;
        std::vector<std::string> channels = split(token, ',');

        p->channels = validChannelName(channels);
        if (!isEOF() && getToken()) {
            std::vector<std::string> keys = split(token, ',');
            p->keys = keys;
        }
    } else {
        throw NotEnoughParamsException("JOIN");
    }
    params = p;
}
void Parser::parsePart(e_cmd &cmd, params *&params) {
    cmd = PART;
    part_params *p;
    if (!isEOF() && getToken()) {
        p = new part_params;
        std::vector<std::string> channels = split(token, ',');

        p->channels = validChannelName(channels);
    } else {
        throw NotEnoughParamsException("PART");
    }
    params = p;
}
void Parser::parseMode(e_cmd &cmd, params *&params) {
    cmd = MODE;
    mode_params *p;

    if (!isEOF() && getToken()) {
        p = new mode_params;
        p->channel = token;
        if (token[0] != '#') throw UnHandledModeException("MODE");
        if (!isEOF() && getToken()) {
            if (token == "+o" || token == "o") {
                p->mode = OPER_T;
                if (!isEOF() && getToken()) {
                    p->nickname = validNickName(token);
                } else {
                    delete p;
                    throw NotEnoughParamsException("MODE");
                }
            } else if (token == "-o") {
                p->mode = OPER_F;
                if (!isEOF() && getToken()) {
                    p->nickname = validNickName(token);
                } else {
                    delete p;
                    throw NotEnoughParamsException("MODE");
                }
            } else if (token == "+i" || token == "i")
                p->mode = INVITE_ONLY_T;
            else if (token == "-i")
                p->mode = INVITE_ONLY_F;
            else if (token == "+t" || token == "t")
                p->mode = TOPIC_PRIV_T;
            else if (token == "-t")
                p->mode = TOPIC_PRIV_F;
            else
                throw UnHandledModeException("MODE");
        } else {
            delete p;
            throw UnHandledModeException("MODE");
        }
    } else {
        delete p;
        throw NotEnoughParamsException("MODE");
    }
    params = p;
}
void Parser::parseInvite(e_cmd &cmd, params *&params) {
    cmd = INVITE;
    invite_params *p;
    std::vector<std::string> tokens = split(tokenStream, ' ');

    if (tokens.size() != 2) throw NotEnoughParamsException("INVITE");
    p = new invite_params;
    // TODO fail 시 메모리 해제
    p->nickname = validNickName(tokens[0]);
    p->channel = validChannelName(tokens[1]);
    params = p;
}
void Parser::parseKick(e_cmd &cmd, params *&params) {
    cmd = KICK;
    kick_params *p;

    if (!isEOF() && getToken()) {
        p = new kick_params;

        p->channel = validChannelName(token);
        if (!isEOF() && getToken()) {
            p->user = token;
            if (getToken(MSG) != EOF && token[0] == ':') {
                p->comment = token.substr(1);
            }
        } else {
            delete p;
            throw NotEnoughParamsException("KICK");
        }
    } else {
        throw NotEnoughParamsException("KICK");
    }
    params = p;
}
void Parser::parseTopic(e_cmd &cmd, params *&params) {
    cmd = TOPIC;
    topic_params *p;

    std::vector<std::string> tokens = split(tokenStream, ' ');
    if (!(tokens.size() == 1 || tokens.size() == 2)) {
        throw NotEnoughParamsException("TOPIC");
    }
    p = new topic_params;

    p->channel = validChannelName(tokens[0]);
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
        if (!isEOF() && getToken(MSG) && token[0] == ':') {
            p->msg = token.substr(1);
        } else {
            delete p;
            throw NotEnoughParamsException("PRIVMSG");
        }
    } else {
        throw NotEnoughParamsException("PRIVMSG");
    }
    params = p;
}
void Parser::parseNotice(e_cmd &cmd, params *&params) {
    cmd = NOTICE;
    notice_params *p;

    if (!isEOF() && getToken()) {
        p = new notice_params;
        p->nickname = token;
        if (!isEOF() && getToken(MSG) && token[0] == ':') {
            p->msg = token.substr(1);
        } else {
            delete p;
            throw NotEnoughParamsException("NOTICE");
        }
    } else {
        delete p;
        throw NotEnoughParamsException("NOTICE");
    }
    params = p;
}
void Parser::parsePing(e_cmd &cmd, params *&params) {
    cmd = PING;
    ping_params *p;

    if (!isEOF() && getToken()) {
        p = new ping_params;
        p->servername = token;
    } else {
        delete p;
        throw NotEnoughParamsException("PING");
    }
    params = p;
}

Command *Parser::parse(const std::string &command_line, Command *&command) {
    tokenStream.clear();
    tokenStream.str(command_line);
    getToken();

    if (token == "QUIT") {
        parseQuit(command->type, command->params);
    } else if (token == "PASS") {
        parsePass(command->type, command->params);
    } else if (token == "USER") {
        parseUser(command->type, command->params);
    } else if (token == "NICK") {
        parseNick(command->type, command->params);
    } else if (token == "JOIN") {
        parseJoin(command->type, command->params);
    } else if (token == "PART") {
        parsePart(command->type, command->params);
    } else if (token == "MODE") {
        parseMode(command->type, command->params);
    } else if (token == "INVITE") {
        parseInvite(command->type, command->params);
    } else if (token == "KICK") {
        parseKick(command->type, command->params);
    } else if (token == "TOPIC") {
        parseTopic(command->type, command->params);
    } else if (token == "PRIVMSG") {
        parsePrivmsg(command->type, command->params);
    } else if (token == "NOTICE") {
        parseNotice(command->type, command->params);
    } else if (token == "PING") {
        parsePing(command->type, command->params);
    } else if (token == "CAP" || token == "WHO") {
        delete command;
        command = NULL;
    } else {
        throw UnknownCommandException(token);
    }
    return command;
}
std::queue<Command *> Parser::parse(ConnectSocket *src) {
    std::string line;
    std::queue<Command *> commands;
    std::vector<std::string> command_lines;

    line = src->readRecvBuf();

    if (src->delimiter == CRLF)
        command_lines = split(line, "\r\n");
    else
        command_lines = split(line, '\n');

    std::vector<std::string>::iterator it;
    for (it = command_lines.begin(); it != command_lines.end(); it++) {
        Command *command = new Command;
        try {
            parse(*it, command);
            if (command != NULL) commands.push(command);
        } catch (SyntaxException &e) {
            delete command;
            ErrorHandler::handleError(e, src);
        }
    }
    return commands;
}

Parser::SyntaxException::SyntaxException(const std::string &cause)
    : _cause(cause) {}
Parser::SyntaxException::~SyntaxException() throw() {}
std::string Parser::SyntaxException::getCause() { return _cause; }

}  // namespace ft
