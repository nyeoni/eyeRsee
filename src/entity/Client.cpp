#include "entity/Client.hpp"

namespace ft {
Client::Client() {}

Client::Client(const std::string &nickname) : _nickname(nickname) {}

Client::Client(const Client &copy) { *this = copy; }

Client::~Client() {}

Client &Client::operator=(const Client &ref) { return (*this); }

// getter
std::set<Channel *> Client::getChannels() const { return _channels; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getHostname() const { return _hostname; }
std::string Client::getRealname() const { return _realname; }

// setter
void Client::setChannels(Channel *channel) {}
void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
void Client::setHostname(const std::string &hostname) { _hostname = hostname; }
void Client::setRealname(const std::string &realname) { _realname = realname; }

// compare operators
bool Client::operator==(const Client &other) const {
    return _nickname == other._nickname;
}

bool Client::operator!=(const Client &other) const {
    return _nickname != other._nickname;
}
bool Client::operator<(const Client &other) const {
    return _nickname < other._nickname;
}

bool Client::operator>(const Client &other) const {
    return _nickname > other._nickname;
}

bool Client::operator<=(const Client &other) const {
    return _nickname <= other._nickname;
}

bool Client::operator>=(const Client &other) const {
    return _nickname >= other._nickname;
}

}  // namespace ft