#include "controller/Executor.hpp"

#include <utility>

#include "entity/Channel.hpp"
#include "entity/Client.hpp"
#include "handler/ErrorHandler.hpp"
#include "handler/ResponseHandler.hpp"

namespace ft {

Executor::Executor() {}

Executor::Executor(const Executor &copy) { *this = copy; }

Executor::~Executor() {}

Executor &Executor::operator=(const Executor &ref) {
    this->_client_list = ref._client_list;
    this->channel_controller = ref.channel_controller;
    this->client_controller = ref.client_controller;
    this->bot_controller = ref.bot_controller;

    return (*this);
}

Channel *Executor::createChannel(std::string channel_name) {
    return channel_controller.insert(channel_name);
}

const std::set<Client *> &Executor::getClientList() const {
    return _client_list;
}
void Executor::clearClientList() { _client_list.clear(); }

int Executor::updateClientStatus(int fd, Client *client, e_status status) {
    Client *target = client_controller.find(fd);

    if (!target || target != client) return -1;
    if (status == TIMEOUT && client->getStatus() != UNREGISTER) return -1;
    if (status == REGISTER && client->isAuthenticate() == false) return -1;
    client_controller.updateClientStatus(client, status);
    return 0;
}

void Executor::deleteClient(Client *client) {
    ChannelController::ChannelList channel_list;

    client_controller.findInSet(channel_list, client);
    channel_controller.eraseClient(channel_list, client);
    client_controller.erase(client->getFd());
}

Client *Executor::accept(int fd) {
    ConnectSocket tmp;
    Client *new_client;

    tmp.createSocket(fd);
    new_client = client_controller.insert(tmp.getFd());
    return new_client;
}

bool Executor::connect(Client *client, std::string password) {
    std::queue<Command *> &commands = client->commands;
    while (commands.size()) {
        execute(commands.front(), client, password);
        commands.pop();
        if (client->isAuthenticate()) {
            updateClientStatus(client->getFd(), client, REGISTER);
            return true;
        }
    }
    return false;
}

void Executor::execute(Command *command, Client *client, std::string password) {
    switch (command->type) {
        case PASS:
            pass(client, command->params, password);
            break;
        case USER:
            user(client, command->params);
            break;
        case NICK:
            nick(client, command->params);
            break;
        default:
            ErrorHandler::handleError(client, command->getCommand(),
                                      ERR_NOTREGISTERED);
            break;
    }
    delete command;
}

void Executor::execute(Command *command, Client *client) {
    switch (command->type) {
        case NICK:
            nick(client->getFd(), command->params);
            break;
        case JOIN:
            join(client, command->params);
            break;
        case PART:
            part(client, command->params);
            break;
        case MODE:
            mode(client, command->params);
            break;
        case INVITE:
            invite(client, command->params);
            break;
        case KICK:
            kick(client, command->params);
            break;
        case PRIVMSG:
            privmsg(client, command->params);
            break;
        case TOPIC:
            topic(client, command->params);
            break;
        case QUIT:
            quit(client, command->params);
            break;
        case NOTICE:
            notice(client, command->params);
            break;
        case PING:
            pong(client);
            break;
        case BOT:
            bot(client, command->params);
            break;
        default:
            break;
    }
    delete command;
}

/**
 * @brief part channels
 *
 * @param fd
 * @param channels 여러 개의 channel
 */
void Executor::part(Client *client, params *params) {
    part_params *param = dynamic_cast<part_params *>(params);
    std::vector<std::string> channels = param->channels;
    std::vector<std::string>::iterator iter = channels.begin();
    Channel *channel;

    for (; iter != channels.end(); ++iter) {
        channel = channel_controller.find(*iter);
        if (channel) {
            std::string response_msg = ResponseHandler::createResponse(
                client, "PART", channel->getName());
            broadcast(channel, response_msg);
            client_controller.eraseChannel(client, channel);
            channel_controller.eraseClient(channel, client);
        } else {
            // 403 nick3 #bye :No such channel
            ErrorHandler::handleError(client, *iter, ERR_NOSUCHCHANNEL);
        }
    }
}

void Executor::join(Client *client, params *params) {
    join_params *param = dynamic_cast<join_params *>(params);
    std::vector<std::string> channels = param->channels;
    std::vector<std::string>::iterator iter = channels.begin();
    Channel *channel;
    std::string response_msg;

    for (; iter != channels.end(); iter++) {
        channel = channel_controller.find(*iter);
        if (channel == NULL) {  // new channel & operator Client
            channel = channel_controller.insert(*iter);
            channel_controller.insertOperator(channel, client);
        } else if (channel_controller.isOnChannel(channel, client)) {
            return;
        } else if (channel_controller.isInviteMode(channel) &&
                   !channel_controller.isInvitedClient(channel, client)) {
            ErrorHandler::handleError(client, *iter, ERR_INVITEONLYCHAN);
            return;
        } else {  // regular Client
            channel_controller.insertRegular(channel, client);
            if (channel_controller.isInvitedClient(channel, client))
                channel_controller.eraseInvitedClient(channel, client);
        }
        client_controller.insertChannel(client, channel);

        // response
        response_msg =
            ResponseHandler::createResponse(client, "JOIN", channel->getName());
        broadcast(channel, response_msg);
        if (channel != NULL) {  // join한 client
            std::vector<std::string> operators =
                client_controller.clientToString(
                    channel_controller.getOperators(channel));

            std::vector<std::string> regulars =
                client_controller.clientToString(
                    channel_controller.getRegulars(channel));

            response_msg = ResponseHandler::createJoinReponse(
                *iter, client->getNickname(), operators, regulars);

            client->send_buf.append(response_msg);
        }
    }
}

void Executor::mode(Client *client, params *params) {
    const static std::string mode_str[] = {"-o", "+o", "-i", "+i", "-t", "+t"};
    mode_params *param = dynamic_cast<mode_params *>(params);
    std::string channel_name = param->channel;
    e_mode mode = param->mode;
    Channel *channel = channel_controller.find(channel_name);
    std::string response_msg;

    // 403 No such channel
    if (channel == NULL) {
        ErrorHandler::handleError(client, channel_name, ERR_NOSUCHCHANNEL);
        return;
    }
    // 442 Not on channel
    if (!channel_controller.isOnChannel(channel, client)) {
        ErrorHandler::handleError(client, channel_name, ERR_NOTONCHANNEL);
        return;
    }
    // 482 permission
    if (!channel_controller.isOperator(channel, client)) {
        ErrorHandler::handleError(client, channel_name, ERR_CHANOPRIVSNEEDED);
        return;
    }

    if (mode == OPER_F || mode == OPER_T) {  // +o, -o
        Client *target = client_controller.find(param->nickname);
        if (target == NULL) {
            // target no such nickname
            ErrorHandler::handleError(client, param->nickname, ERR_NOSUCHNICK);
            return;
        }
        if (channel_controller.updateMode(mode, channel, target) == false) {
            return;
        }
        response_msg = ResponseHandler::createResponse(
            client, "MODE", channel_name + " " + mode_str[mode],
            param->nickname);
    } else {  // +i, -i, +t, -t
        if (channel_controller.updateMode(mode, channel) == false) {
            return;
        }
        response_msg = ResponseHandler::createResponse(
            client, "MODE", channel_name, mode_str[mode]);
    }

    // response
    broadcast(channel, response_msg);
}

void Executor::topic(Client *client, params *params) {
    topic_params *param = dynamic_cast<topic_params *>(params);
    std::string channel_name = param->channel;
    std::string topic = param->topic;
    Channel *channel = channel_controller.find(channel_name);

    // 403 nick2 hi : No such channel
    if (channel == NULL) {
        ErrorHandler::handleError(client, channel_name, ERR_NOSUCHCHANNEL);
        return;
    }

    // 442 nick2 #channel :You're not on that channel!
    if (!channel_controller.isOnChannel(channel, client)) {
        ErrorHandler::handleError(client, channel_name, ERR_NOTONCHANNEL);
        return;
    }

    // 482 nick2 #channel :You must be a channel op or higher to change
    if (channel_controller.isTopicMode(channel) &&
        !channel_controller.isOperator(channel, client)) {
        ErrorHandler::handleError(client, channel_name, ERR_CHANOPRIVSNEEDED);
        return;
    }

    std::string prev_topic = channel->getTopic();
    if (prev_topic == topic) return;
    channel_controller.updateTopic(channel, topic);
    std::string response_msg =
        ResponseHandler::createResponse(client, "TOPIC", channel_name, topic);
    broadcast(channel, response_msg);
}

void Executor::invite(Client *invitor, params *params) {
    invite_params *param = dynamic_cast<invite_params *>(params);
    std::string nickname = param->nickname;
    std::string channel_name = param->channel;
    Client *target = client_controller.find(nickname);
    Channel *channel = channel_controller.find(channel_name);
    std::string response_msg;

    //  403 user2 #testchannel :No such channel
    if (channel == NULL) {
        ErrorHandler::handleError(invitor, channel_name, ERR_NOSUCHCHANNEL);
        return;
    }
    // 401 nick2 hi :No such nick
    if (target == NULL) {
        ErrorHandler::handleError(invitor, nickname, ERR_NOSUCHNICK);
        return;
    }
    // 442 nick2 #channel :You're not on that channel!
    if (!channel_controller.isOnChannel(channel, invitor)) {
        ErrorHandler::handleError(invitor, channel_name, ERR_NOTONCHANNEL);
        return;
    }
    // 482 nick2 #channel :You must be a channel op or higher to send an invite.
    if (!channel_controller.isOperator(channel, invitor)) {
        ErrorHandler::handleError(invitor, channel_name, ERR_CHANOPRIVSNEEDED);
        return;
    }

    channel_controller.insertInvitedClient(channel, target);
    ResponseHandler::handleInviteResponse(invitor, target, channel_name);
    _client_list.insert(target);
}

void Executor::pass(Client *new_client, params *params,
                    std::string server_password) {
    std::string password = dynamic_cast<pass_params *>(params)->password;

    if (new_client->auth[PASS]) {
        return ErrorHandler::handleError(new_client, new_client->getNickname(),
                                         ERR_ALREADYREGISTERED);
    }
    if (password != server_password) {
        return ErrorHandler::handleError(new_client, "*", ERR_PASSWDMISMATCH);
    }
    new_client->auth[PASS] = true;
}

void Executor::user(Client *new_client, params *params) {
    user_params *param = dynamic_cast<user_params *>(params);

    if (new_client->auth[USER]) {
        // 462 abc :You may not reregister.
        ErrorHandler::handleError(new_client, new_client->getNickname(),
                                  ERR_ALREADYREGISTERED);
        return;
    }
    new_client->setUsername(param->username);
    new_client->setHostname(param->hostname);
    new_client->setServername(param->servername);
    new_client->setRealname(param->realname);
    new_client->auth[USER] = true;
}

/**
 * @brief only used in connection registration
 */
void Executor::nick(Client *new_client, params *params) {
    std::string nickname = dynamic_cast<nick_params *>(params)->nickname;

    if (client_controller.find(nickname)) {
        // 433 user2 user1 :Nickname is already in use.
        std::string cause = new_client->getNickname() + " " + nickname;
        ErrorHandler::handleError(new_client, cause, ERR_NICKNAMEINUSE);
    } else {
        new_client->setNickname(nickname);
        new_client->auth[NICK] = true;
    }
}

/**
 * @brief only used in handleExecute
 */
void Executor::nick(int fd, params *params) {
    std::string nickname = dynamic_cast<nick_params *>(params)->nickname;
    Client *client = client_controller.find(fd);

    if (client->getNickname() == nickname) return;
    if (client_controller.find(nickname)) {
        std::string cause = client->getNickname() + " " + nickname;
        ErrorHandler::handleError(client, cause, ERR_NICKNAMEINUSE);
        return;
    }
    if (client->getChannelList().empty()) {
        ResponseHandler::handleResponse(client, "NICK", nickname);
    } else {
        std::string response_msg =
            ResponseHandler::createResponse(client, "NICK", nickname);
        broadcast(client->getChannelList(), response_msg);
    }
    client_controller.updateNickname(client, nickname);
}

void Executor::quit(Client *client, params *params) {
    std::string msg = dynamic_cast<quit_params *>(params)->msg;
    ChannelController::ChannelList channel_list;
    std::string response_msg;

    if (msg.length() == 0) msg = "Quit: leaving";
    response_msg = ResponseHandler::createResponse(client, "QUIT", msg);

    // 모든 채널에서 quit && send message
    _client_list.insert(client);
    broadcast(client->getChannelList(), response_msg);
    client_controller.updateClientStatus(client, TERMINATE);
}

void Executor::kick(Client *kicker, params *params) {
    kick_params *param = dynamic_cast<kick_params *>(params);
    std::string channel_name = param->channel;
    std::string nickname = param->user;
    std::string comment = param->comment.empty() ? " " : param->comment;
    Client *client = client_controller.find(nickname);
    Channel *channel = channel_controller.find(channel_name);

    //  403 user2 #testchannel :No such channel
    if (channel == NULL) {
        ErrorHandler::handleError(kicker, channel_name, ERR_NOSUCHCHANNEL);
        return;
    }
    // 401 user nickname :No such user
    if (client == NULL) {
        ErrorHandler::handleError(kicker, nickname, ERR_NOSUCHNICK);
        return;
    }
    // 442 not on channel
    if (!channel_controller.isOnChannel(channel, kicker)) {
        ErrorHandler::handleError(kicker, channel_name, ERR_NOTONCHANNEL);
        return;
    }
    // 482 not channel operator
    if (!channel_controller.isOperator(channel, kicker)) {
        ErrorHandler::handleError(kicker, channel_name, ERR_CHANOPRIVSNEEDED);
        return;
    }

    // response
    std::string response_msg = ResponseHandler::createResponse(
        kicker, "KICK", channel_name + " " + nickname, comment);
    broadcast(channel, response_msg);

    channel_controller.eraseClient(channel, client);
    client_controller.eraseChannel(client, channel);
}

void Executor::privmsg(Client *client, params *params) {
    privmsg_params *param = dynamic_cast<privmsg_params *>(params);
    std::vector<std::string> receivers = param->receivers;
    std::vector<std::string>::iterator iter = receivers.begin();
    std::string name;

    for (; iter != receivers.end(); ++iter) {
        name = *iter;
        if (iter->front() == '#') {  // channel
            Channel *channel = channel_controller.find(name);
            if (channel) {
                if (channel_controller.isOnChannel(channel, client)) {
                    std::string response_msg = ResponseHandler::createResponse(
                        client, "PRIVMSG", name, param->msg);
                    broadcast(channel, response_msg, client);
                } else {
                    //  404 You cannot send external  messages
                    ErrorHandler::handleError(client, name,
                                              ERR_CANNOTSENDTOCHAN);
                    return;
                }
            } else {
                //  403 user2 #testchannel :No such channel
                ErrorHandler::handleError(client, name, ERR_NOSUCHCHANNEL);
                return;
            }
        } else {  // user
            Client *receiver = client_controller.find(name);
            if (receiver) {
                std::string response_msg = ResponseHandler::createResponse(
                    client, "PRIVMSG", name, param->msg);
                receiver->send_buf.append(response_msg);
                _client_list.insert(receiver);
            } else {
                // 401 user3 wow :No such nick
                ErrorHandler::handleError(client, name, ERR_NOSUCHNICK);
                return;
            }
        }
    }
}

/**
 * @brief channel에 가입한 모든 clients에게 broadcast
 */
void Executor::broadcast(Channel *channel, const std::string &msg,
                         Client *excluded) {
    std::set<Client *> receivers;
    std::set<Client *>::iterator iter;

    channel_controller.findInSet(receivers, channel);
    if (excluded) receivers.erase(excluded);
    iter = receivers.begin();
    for (; iter != receivers.end(); ++iter) {
        (*iter)->send_buf.append(msg);
    }
    _client_list.insert(receivers.begin(), receivers.end());
}

/**
 * @brief client가 가입한 모든 채널에 broadcast (메시지 중복 X)
 */
void Executor::broadcast(const ChannelList &channel_list,
                         const std::string &msg) {
    ClientList client_list;
    client_list_iterator client_iter;
    channel_list_iterator channel_iter = channel_list.begin();

    for (; channel_iter != channel_list.end(); ++channel_iter) {
        channel_controller.findInSet(client_list, *channel_iter);
    }
    client_iter = client_list.begin();
    for (; client_iter != client_list.end(); ++client_iter) {
        (*client_iter)->send_buf.append(msg);
    }
    _client_list.insert(client_list.begin(), client_list.end());
}

void Executor::notice(Client *client, params *params) {
    notice_params *param = dynamic_cast<notice_params *>(params);
    Client *receiver = client_controller.find(param->nickname);

    if (receiver) {
        ResponseHandler::handleResponse(receiver, "NOTICE",
                                        client->getNickname(), param->msg);
        _client_list.insert(receiver);
    } else
        ErrorHandler::handleError(client, param->nickname, ERR_NOSUCHNICK);
}

void Executor::pong(Client *client) {
    ResponseHandler::handlePongResponse(client);
}

void Executor::bot(Client *client, params *params) {
    e_bot_cmd cmd = dynamic_cast<bot_params *>(params)->cmd;
    std::string receiver = dynamic_cast<bot_params *>(params)->receiver;
    std::string msg;
    std::string delimiter = client->delimiter == CRLF ? "\r\n" : "\n";
    switch (cmd) {
        case BOT_HELP:
            msg = bot_controller.help();
            break;
        case BOT_NOW:
            msg = bot_controller.now();
            break;
        case BOT_HI:
            msg = bot_controller.hi();
            break;
        case BOT_BYE:
            msg = bot_controller.bye();
            break;
        case BOT_WEATHER:
            msg = bot_controller.weather();
            break;
        case BOT_NKIM:
            msg = bot_controller.nkim();
            break;
        case BOT_HANNKIM:
            msg = bot_controller.hannkim();
            break;
        case BOT_CHAEJKIM:
            msg = bot_controller.chaejkim();
            break;
        case BOT_EYERSEE:
            msg = bot_controller.eyersee();
            break;
        default:
            break;
    }
    ResponseHandler::handleBotResponse(client, receiver, msg);
}

}  // namespace ft
