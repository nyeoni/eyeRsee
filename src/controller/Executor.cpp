#include "controller/Executor.hpp"

#include <iostream>
#include <utility>

#include "entity/Channel.hpp"
#include "entity/Client.hpp"

namespace ft {
Executor::Executor() {}

Executor::Executor(const Executor &copy) { *this = copy; }

Executor::~Executor() {}

Executor &Executor::operator=(const Executor &ref) { return (*this); }

Channel *Executor::createChannel(std::string channel_name) {
    return channel_controller.insert(channel_name);
}

void Executor::deleteClient(Client *client) {
    channel_controller.eraseClient(client);
    client_controller.erase(client->getFd());
}

Client *Executor::accept(int fd) {
    ConnectSocket tmp;
    Client *new_client;

    tmp.createSocket(fd);
    new_client = client_controller.insert(tmp.getFd());
    return new_client;
}

void Executor::connect(Command *command, Client *client, std::string password) {
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
            std::cout << ":NAYEON.local 451 * JOIN :You have not registered."
                      << std::endl;
            break;
    }
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
            // case NOTICE:
            //      notice(fd);
            //     break;
            // case PING:
            //      ping(fd);
            //     break;
            // case PONG:
            //      pong(fd);
            //     break;
        default:
            break;
    }
}

/**
 * @brief part channels
 *
 * @param fd
 * @param channels 여러 개의 channel
 */
void Executor::part(Client *client, params *params) {
    std::vector<std::string> channels =
        dynamic_cast<part_params *>(params)->channels;
    std::vector<std::string>::iterator iter = channels.begin();
    Channel *channel;

    for (; iter != channels.end(); ++iter) {
        channel = channel_controller.find(*iter);
        if (channel) {
            client_controller.eraseChannel(client, channel);
            channel_controller.eraseClient(channel, client);
            channel_controller.broadcast(channel);
        } else {
            // ErrorHandler::handleResponse();
            // ErrorHandler::handleError();
            //            ErrorHandler::handleError("asdfasdf")
            // 403 nick3 #bye :No such channel
        }
    }
}

void Executor::join(Client *client, params *params) {
    std::vector<std::string> channels =
        dynamic_cast<join_params *>(params)->channels;
    std::vector<std::string>::iterator iter = channels.begin();
    for (; iter != channels.end(); iter++) {
        Channel *channel = channel_controller.find(*iter);
        if (channel == NULL) {  // new channel
            channel = channel_controller.insert(*iter);
            //                channel_controller.insert(*iter);
            channel_controller.insertClient(channel, client, true);
        } else {
            channel_controller.insertClient(channel, client, false);
        }
        client_controller.insertChannel(client, channel);
        channel_controller.broadcast(channel, "join msg");
    }
}

void Executor::mode(Client *client, params *params) {
    std::string channel_name = dynamic_cast<mode_params *>(params)->channel;
    e_mode mode = dynamic_cast<mode_params *>(params)->mode;
    Channel *channel = channel_controller.find(channel_name);

    channel_controller.updateMode(mode, channel_name);
    channel_controller.broadcast(channel);
}

void Executor::topic(Client *client, params *params) {
    std::string channel_name = dynamic_cast<topic_params *>(params)->channel;
    std::string topic = dynamic_cast<topic_params *>(params)->topic;
    Channel *channel = channel_controller.find(channel_name);

    if (channel && client) {
        channel_controller.updateTopic(client, channel, topic);
        channel_controller.broadcast(channel);
    } else {
        // error
    }
}

void Executor::invite(Client *invitor, params *params) {
    std::string nickname = dynamic_cast<invite_params *>(params)->nickname;
    std::string channel_name = dynamic_cast<invite_params *>(params)->channel;
    Client *client = client_controller.find(nickname);
    Channel *channel = channel_controller.find(channel_name);

    if (channel == NULL) {
        //  403 user2 #testchannel :No such channel
        return;
    }
    if (client == NULL) {
        // No such user
        return;
    }
    if (channel_controller.hasPermission(channel, invitor))
        client_controller.insertInviteChannel(client, channel);
    else {
        // error - hasPermission can generate error message code
    }
}

void Executor::pass(Client *new_client, params *params,
                    std::string server_password) {
    std::string password = dynamic_cast<pass_params *>(params)->password;

    if (new_client->auth[PASS]) {
        // already auth
        return;
    }
    if (password == server_password)
        new_client->auth[PASS] = true;
    else
        new_client->auth[PASS] = false;
}
void Executor::user(Client *new_client, params *params) {
    user_params *param = dynamic_cast<user_params *>(params);
    std::string username = param->username;
    std::string hostname = param->hostname;
    std::string server = param->servername;
    std::string realname = param->realname;

    if (new_client->auth[USER]) {
        // 462 abc :You may not reregister.
        return;
    }
    new_client->setUsername(username);
    new_client->setHostname(hostname);
    new_client->setServer(server);
    new_client->setRealname(realname);
    new_client->auth[USER] = true;
    // 461 abc USER :Not enough parameters. -> parser
}

/**
 * @brief only used in handleConnect
 */
void Executor::nick(Client *new_client, params *params) {
    std::string nickname = dynamic_cast<nick_params *>(params)->nickname;

    if (client_controller.find(nickname)) {
        // 433 user2 user1 :Nickname is already in use.
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

    if (client_controller.find(nickname)) {
        // 433 user2 user1 :Nickname is already in use.
        return;  // throw
    }
    client_controller.updateNickname(client, nickname);
    client_controller.broadcast(client);
}
void Executor::quit(Client *client, params *params) {
    std::string msg = dynamic_cast<quit_params *>(params)->msg;
    // 모든 채널에서 quit && send message
    client_controller.broadcast(client, msg);
    client_controller.erase(client);
    channel_controller.eraseClient(client);
}
void Executor::kick(Client *kicker, params *params) {
    kick_params *param = dynamic_cast<kick_params *>(params);
    std::string channel_name = param->channel;
    std::string nickname = param->user;  // TODO param->user nickname?
    std::string comment = param->comment;
    Client *client = client_controller.find(nickname);
    Channel *channel = channel_controller.find(channel_name);

    if (channel == NULL) {
        //
        //  403 user2 #testchannel :No such channel

        return;
    }
    if (client == NULL) {
        // No such user
        return;
    }
    if (channel_controller.hasPermission(channel, kicker)) {
        channel_controller.eraseClient(channel, client);
        client_controller.eraseChannel(client, channel);
        channel_controller.broadcast(channel);
    } else {
        // error - hasPermission can generate error message code
    }
}

void Executor::privmsg(Client *client, params *params) {
    std::string name;
    std::vector<std::string> receivers =
        dynamic_cast<privmsg_params *>(params)->receivers;
    std::string msg = dynamic_cast<privmsg_params *>(params)->msg;
    std::vector<std::string>::iterator iter = receivers.begin();

    for (; iter != receivers.end(); ++iter) {
        if (iter->front() == '#') {  // channel
            name = (*iter).at(1);
            Channel *channel = channel_controller.find(name);
            if (channel) {
                if (channel->isOnChannel(client)) {
                    // user3!hannah@127.0.0.1 PRIVMSG #testchannel :hi
                    channel_controller.broadcast(channel, msg, client);
                } else {
                    //  404 You cannot send external  messages to this
                    //  channel
                }
            } else {
                //  403 user2 #testchannel :No such channel
            }
        } else {  // user
            name = *iter;
            Client *receiver = client_controller.find(name);
            if (receiver) {
                receiver->send_buf.append("privmsg : " + msg);
                // send(client_controller.find(name)->getFd(), msg.c_str(),
                //      msg.length(), 0);
            } else {
                // 401 user3 wow :No such nick
            }
        }
    }
}

}  // namespace ft
