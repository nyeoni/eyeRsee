#include "controller/ClientController.hpp"

#include <utility>  // std::make_pair

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
    return &(iter->second);
}

Client *ClientController::find(const Client *client) {
    client_iterator iter = _clients.find(client->getFd());
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
 * @brief create client to _clients
 *
 * @param client
 */
void ClientController::create(const Client *client) {
    _clients.insert(std::make_pair(client->getFd(), *client));
}

void ClientController::create(int fd, const Client *client) {
    _clients.insert(std::make_pair(fd, *client));
}

/**
 * @brief delete client from _clients
 *
 * @param client
 */
void ClientController::del(const Client *client) {
    _clients.erase(client->getFd());
}

void ClientController::del(const std::string &nickname) {
    Client *target = find(nickname);

    if (target) {
        _clients.erase(target->getFd());
    } else {
        // TODO there are no `nickname`
    }
}

void ClientController::del(int fd) {
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
void ClientController::update(int fd, const std::string &nickname) {
    Client *client = find(fd);

    if (client) {  // valid
        if (find(nickname)) {
            // no change
        } else {
            // change nickname
            client->setNickname(nickname);
        }
    }
    // TODO error handling (잘못된 fd일 경우)
}

void ClientController::update(Client *client, const std::string &nickname) {
    if (find(nickname)) {
        // no change
    } else {
        client->setNickname(nickname);
    }
}

void ClientController::updateClient(Client *client, Channel *channel,
                                    bool is_insert) {
    client->updateChannelList(channel, is_insert);
}

}  // namespace ft
