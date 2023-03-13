#include "controller/ClientController.hpp"

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

// map<std::string, Client>
// nickname,
/**
 * @brief find client in _clients
 *
 * @param client
 * @return ClientController::client_iterator
 */
ClientController::client_iterator ClientController::findClient(Client *client) {
    // Client *ClientController::findClient(Client *client) {
    // return &(*(_clients.find(tmp)));
    client_iterator iter = _clients.find(*client);
    return iter;
}

ClientController::client_iterator ClientController::findClient(
    const std::string &nickname) {
    Client tmp(nickname);
    client_iterator iter = _clients.find(tmp);
    return iter;
}

/**
 * @brief delete client from _clients
 *
 * @param client
 */
void ClientController::deleteClient(Client *client) { _clients.erase(*client); }
void ClientController::deleteClient(const std::string &nickname) {
    Client tmp(nickname);
    _clients.erase(tmp);
}

/**
 * @brief add client to _clients
 *
 * @param client
 */
void ClientController::addClient(Client *client) { _clients.insert(*client); }
void ClientController::addClient(const std::string &nickname) {
    Client tmp(nickname);
    _clients.insert(tmp);
}

}  // namespace ft
