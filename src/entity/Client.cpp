#include "entity/Client.hpp"

namespace ft {
Client::Client() {}

Client::Client(const std::string &nickname) : _nickname(nickname) {}

Client::Client(const Client &copy) { *this = copy; }

Client::~Client() {}

Client &Client::operator=(const Client &ref) { return (*this); }

// getter
Client::ChannelList Client::getChannelList() const { return _channel_list; }
std::string Client::getNickname() const { return _nickname; }
std::string Client::getHostname() const { return _hostname; }
std::string Client::getRealname() const { return _realname; }

// setter
void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
void Client::setHostname(const std::string &hostname) { _hostname = hostname; }
void Client::setRealname(const std::string &realname) { _realname = realname; }

// update
void Client::insertChannel(Channel *channel) { _channel_list.insert(channel); }
void Client::insertInviteChannel(Channel *channel) {
    _i_channel_list.insert(channel);
}

void Client::eraseChannel(Channel *channel) { _channel_list.erase(channel); }

void Client::clearChannel() { _channel_list.clear(); }

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
