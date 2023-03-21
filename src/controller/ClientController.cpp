#include "controller/ClientController.hpp"

#include <utility>  // std::make_pair

#include "entity/Channel.hpp"
#include "entity/Client.hpp"

namespace ft {
ClientController::ClientController() {}

ClientController::ClientController(const ClientController &copy) {
    *this = copy;
}

ClientController::~ClientController() {}

ClientController &ClientController::operator=(const ClientController &ref) {
    return (*this);
}

/**
 * @brief find client in _clients
 *
 * @param client
 * @return ClientController::client_iterator
 */
Client *ClientController::find(const int fd) {
    client_iterator iter = _clients.find(fd);
    if (iter == _clients.end()) return NULL;
    return &(iter->second);
}

Client *ClientController::find(const Client *client) {
    client_iterator iter = _clients.find(client->getFd());
    if (iter == _clients.end()) return NULL;
    return &(iter->second);
}

// check duplicated nickname
Client *ClientController::find(const std::string &nickname) {
    client_iterator iter = _clients.begin();

    for (; iter != _clients.end(); ++iter) {
        if (iter->second.getNickname() == nickname) {
            return &(iter->second);
        }
    }

    return NULL;
}

/**
 * @brief find client and put channel_list
 *
 * @param client
 * @return ClientController::client_iterator
 */
void ClientController::findInSet(Client::ChannelList &channel_list,
                                 Client *client) {
    const ChannelList joined_channels = client->getChannelList();
    channel_list.insert(joined_channels.begin(), joined_channels.end());
}

/**
 * @brief insert client to _clients
 *
 * @param client
 */
Client *ClientController::insert(int fd) {
    Client *new_client;
    pair p = _clients.insert(std::make_pair(fd, Client()));
    new_client = &(p.first->second);
    new_client->setFd(fd);
    return new_client;
}

/**
 * @brief delete client from _clients
 *
 * @param client
 */
void ClientController::erase(const Client *client) {
    _clients.erase(client->getFd());
}

void ClientController::erase(const std::string &nickname) {
    Client *target = find(nickname);

    if (target) {
        _clients.erase(target->getFd());
    } else {
        // TODO there are no `nickname`
    }
}

void ClientController::erase(int fd) {
    Client *target = find(fd);

    if (target) {
        _clients.erase(target->getFd());
    } else {
        // TODO there are no `nickname`
    }
}

/**
 * @brief update nickname
 *
 * @param fd
 * @param nickname
 */
void ClientController::updateNickname(int fd, const std::string &nickname) {
    Client *client = find(fd);

    if (client) {  // valid
        if (find(nickname)) {
            // no change (already exist)
        } else {
            // change nickname
            client->setNickname(nickname);
        }
    }
    // TODO error handling (잘못된 fd일 경우)
}

void ClientController::updateNickname(Client *client,
                                      const std::string &nickname) {
    client->setNickname(nickname);
}

/**
 * @brief insert Channel to Client's _channelList
 */
void ClientController::insertChannel(Client *client, Channel *channel) {
    client->insertChannel(channel);
}

void ClientController::insertInviteChannel(Client *client, Channel *channel) {
    client->insertInviteChannel(channel);
}

/**
 * @brief erase Channel to Client's _channelList
 */
void ClientController::eraseChannel(Client *client, Channel *channel) {
    client->eraseChannel(channel);
}

/**
 * @brief client가 가입한 모든 채널에 broadcast
 */
void ClientController::broadcast(Client *client, std::string msg) {
    Channel::ClientList receivers = findReceivers(client);
    Channel::client_list_iterator client_iter = receivers.begin();

    for (; client_iter != receivers.end(); ++client_iter) {
        // push send_queue
        // (*client_iter)->insertSendQueue(respons);
        // send((*client_iter)->getFd(), msg.c_str(), msg.length(), 0);
        client->send_buf.append("msg : " + msg);
    }
}

/**
 * @brief 클라이언트가 가입한 채널의 모든 Client List 반환 (중복 X)
 *
 * @return ClientList
 * @note ClientController에 옮길 수 있음
 */
std::set<Client *> ClientController::findReceivers(Client *client) {
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
