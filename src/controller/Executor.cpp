#include "controller/Executor.hpp"

#include <utility>

#include "entity/Channel.hpp"
#include "entity/Client.hpp"

namespace ft {
Executor::Executor() {}

Executor::Executor(const Executor &copy) { *this = copy; }

Executor::~Executor() {}

Executor &Executor::operator=(const Executor &ref) { return (*this); }

Client *Executor::creatClient(int fd) { return client_controller.insert(fd); }

/**
 * @brief part channels
 *
 * @param fd
 * @param channels 여러 개의 channel
 */
void Executor::part(int fd, CmdLine channels) {
    Client *client = client_controller.find(fd);
    Channel *channel;
    cmd_iterator iter = channels.begin();

    for (; iter != channels.end(); ++iter) {
        channel = channel_controller.find(*iter);
        if (channel) {
            client_controller.eraseChannel(client, channel);
            channel_controller.eraseClient(channel, client);
            broadcast(channel);
        } else {
            // 403 nick3 #bye :No such channel
        }
    }
}

void Executor::join(int fd, CmdLine cmd_line) {
    Client *client = client_controller.find(fd);
    cmd_iterator iter = cmd_line.begin();
    for (; iter != cmd_line.end(); iter++) {
        if (iter->front() == '#') {
            Channel *channel = channel_controller.find(*iter);
            if (channel == NULL) {  // new channel
                channel_controller.insert(*iter);
                channel_controller.insertClient(channel, client, true);
            } else {
                channel_controller.insertClient(channel, client, false);
            }
            client_controller.insertChannel(client, channel);
            broadcast(channel);
        } else {
            // invalid channel name
        }
    }
}

void Executor::mode(int fd, std::string channel_name, e_mode mode) {
    Channel *channel = channel_controller.find(channel_name);

    channel_controller.updateMode(mode, channel_name);
    broadcast(channel);
}

void Executor::topic(int fd, std::string channel_name, std::string topic) {
    Client *client = client_controller.find(fd);
    Channel *channel = channel_controller.find(channel_name);

    if (channel && client) {
        channel_controller.updateTopic(client, channel, topic);
        broadcast(channel);
    } else {
        // error
    }
}

void Executor::invite(int fd, std::string nickname, std::string channel_name) {
    Client *invitor = client_controller.find(fd);
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

void Executor::pass(Client *new_client, std::string password,
                    std::string server_password) {
    if (new_client->auth[PASS]) {
        // already auth
        return;
    }
    if (password == server_password)
        new_client->auth[PASS] = true;
    else
        new_client->auth[PASS] = false;
}
void Executor::user(Client *new_client, std::string username,
                    std::string hostname, std::string server,
                    std::string realname) {
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
void Executor::nick(Client *new_client, std::string nickname) {
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
void Executor::nick(int fd, std::string nickname) {
    Client *client = client_controller.find(fd);
    if (client_controller.find(nickname)) {
        // 433 user2 user1 :Nickname is already in use.
        return;  // throw
    }
    client_controller.updateNickname(client, nickname);
    broadcast(client);
}
void Executor::quit(int fd, std::string msg) {
    // 모든 채널에서 quit && send message
    Client *client = client_controller.find(fd);

    broadcast(client_controller.find(fd), msg);
    client_controller.erase(fd);
    channel_controller.eraseClient(client);
}
void Executor::kick(int fd, std::string channel_name, std::string nickname,
                    std::string comment) {
    Client *kicker = client_controller.find(fd);
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
        broadcast(channel);
    } else {
        // error - hasPermission can generate error message code
    }
}

void Executor::privmsg(Client *client, CmdLine receivers, std::string msg) {
    std::string name;
    cmd_iterator iter = receivers.begin();

    for (; iter != receivers.end(); ++iter) {
        if (iter->front() == '#') {  // channel
            name = (*iter).at(1);
            Channel *channel = channel_controller.find(name);
            if (channel) {
                if (channel->isOnChannel(client)) {
                    // user3!hannah@127.0.0.1 PRIVMSG #testchannel :hi
                    broadcast(channel, msg, client);
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
                send(client_controller.find(name)->getFd(), msg.c_str(),
                     msg.length(), 0);
            } else {
                // 401 user3 wow :No such nick
            }
        }
    }
}

/**
 * @brief channel에 가입한 모든 clients에게 broadcast
 *
 * @param client - excluded client, 없으면 default NULL
 */
void Executor::broadcast(Channel *channel, std::string msg, Client *client) {
    Channel::ClientList operators = channel->getOperators();
    Channel::ClientList regulars = channel->getRegulars();
    Channel::client_list_iterator iter = operators.begin();

    for (; iter != operators.end(); ++iter) {
        if (client == NULL || client->getFd() != (*iter)->getFd())
            //	client -> send queue -> push_back
            send((*iter)->getFd(), msg.c_str(), msg.length(), 0);
    }
    iter = regulars.begin();
    for (; iter != regulars.end(); ++iter) {
        if (client == NULL || client->getFd() != (*iter)->getFd())
            send((*iter)->getFd(), msg.c_str(), msg.length(), 0);
    }
}

/**
 * @brief client가 가입한 모든 채널에 broadcast
 */
void Executor::broadcast(Client *client, std::string msg) {
    Channel::ClientList receivers = findReceivers(client);
    Channel::client_list_iterator client_iter = receivers.begin();
    for (; client_iter != receivers.end(); ++client_iter) {
        // push send_queue
        // (*client_iter)->insertSendQueue(respons);
        send((*client_iter)->getFd(), msg.c_str(), msg.length(), 0);
    }
}

/**
 * @brief 클라이언트가 가입한 채널의 모든 Client List 반환 (중복 X)
 *
 * @return ClientList
 * @note ClientController에 옮길 수 있음
 */
Executor::ClientList Executor::findReceivers(Client *client) {
    Client::ChannelList channels = client->getChannelList();
    Client::channel_list_iterator channel_iter = channels.begin();
    Channel::ClientList receivers;

    for (; channel_iter != channels.end(); ++channel_iter) {
        {
            Channel::ClientList operators = (*channel_iter)->getOperators();
            Channel::client_list_iterator client_iter = operators.begin();
            for (; client_iter != operators.end(); ++client_iter) {
                receivers.insert(*client_iter);
            }
        }
        {
            Channel::ClientList regulars = (*channel_iter)->getRegulars();
            Channel::client_list_iterator client_iter = regulars.begin();
            for (; client_iter != regulars.end(); ++client_iter) {
                receivers.insert(*client_iter);
            }
        }
    }
    return receivers;
}

}  // namespace ft
