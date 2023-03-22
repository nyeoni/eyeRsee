#include "entity/Client.hpp"

namespace ft {
Client::Client() {}

Client::Client(const Client &copy) { *this = copy; }

Client::~Client() {
    // deleteSocket();
}

Client &Client::operator=(const Client &ref) { return (*this); }

// getter
const Client::ChannelList &Client::getChannelList() const {
    return _channel_list;
}

// update
void Client::insertChannel(Channel *channel) { _channel_list.insert(channel); }
void Client::insertInviteChannel(Channel *channel) {
    _i_channel_list.insert(channel);
}

void Client::eraseChannel(Channel *channel) { _channel_list.erase(channel); }

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
