#include "controller/ClientController.hpp"

#include <utility>

#include "entity/Channel.hpp"
#include "entity/Client.hpp"

namespace ft {
ClientController::ClientController() {}

ClientController::ClientController(const ClientController &copy) {
    *this = copy;
}

ClientController::~ClientController() {}

ClientController &ClientController::operator=(const ClientController &ref) {
    this->_clients = ref._clients;

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
    }
}

void ClientController::erase(int fd) {
    Client *target = find(fd);

    if (target) {
        _clients.erase(target->getFd());
    }
}

void ClientController::updateClientStatus(Client *client, e_status status) {
    client->setStatus(status);
}

/**
 * @brief update nickname
 *
 * @param fd
 * @param nickname
 */
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

/**
 * @brief erase Channel to Client's _channelList
 */
void ClientController::eraseChannel(Client *client, Channel *channel) {
    client->eraseChannel(channel);
}

/**
 * @brief make client list to vector<string>
 */
std::vector<std::string> ClientController::clientToString(
    ClientList client_list) {
    std::vector<std::string> res;
    ClientList::iterator iter = client_list.begin();

    res.reserve(client_list.size());
    for (; iter != client_list.end(); ++iter) {
        res.push_back((*iter)->getNickname());
    }
    return res;
}

}  // namespace ft
