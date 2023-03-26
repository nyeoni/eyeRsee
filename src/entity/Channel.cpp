#include "entity/Channel.hpp"

#include "core/Type.hpp"

namespace ft {

Channel::Channel() {
    _mode = 0;
    setMode(TOPIC_PRIV_T);
    setMode(INVITE_ONLY_F);
}
Channel::Channel(const std::string &name) : _name(name) {
    _mode = 0;
    setMode(TOPIC_PRIV_T);
    setMode(INVITE_ONLY_F);
}
Channel::Channel(const Channel &copy) { *this = copy; }
Channel::~Channel() {}
Channel &Channel::operator=(const Channel &ref) {
    _name = ref._name;
    _topic = ref._topic;
    _regulars = ref._regulars;
    _operators = ref._operators;
    _mode = ref._mode;

    return (*this);
}

const std::string &Channel::getName() const { return _name; }
const std::string &Channel::getTopic() const { return _topic; }
const Channel::ClientList &Channel::getRegulars() const { return _regulars; }
const Channel::ClientList &Channel::getOperators() const { return _operators; }
int Channel::getMode() const { return _mode; }

void Channel::setName(const std::string &name) { _name = name; }
void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setMode(int mode) {
    if (mode % 2)
        _mode |= (1 << (mode / 2));  // +flag
    else
        _mode &= ~(1 << (mode / 2));  // -flag
}

// update
// void Channel::insertClient(Client *client, bool is_operator) {
//     is_operator ? _operators.insert(client) : _regulars.insert(client);
// }

void Channel::insertOperator(Client *client) { _operators.insert(client); }

void Channel::insertRegular(Client *client) { _regulars.insert(client); }

void Channel::insertInvitedClient(Client *client) {
    _invited_clients.insert(client);
}

// void Channel::eraseClient(Client *client, bool is_operator) {
//     is_operator ? _operators.erase(client) : _regulars.erase(client);
// }

void Channel::eraseOperator(Client *client) { _operators.erase(client); }

void Channel::eraseRegular(Client *client) { _regulars.erase(client); }

bool Channel::operator==(const Channel &other) const {
    return (_name == other._name);
}
bool Channel::operator!=(const Channel &other) const {
    return !(*this == other);
}
bool Channel::operator<(const Channel &other) const {
    return (_name < other._name);
}
bool Channel::operator>(const Channel &other) const {
    return (other._name < _name);
}
bool Channel::operator<=(const Channel &other) const {
    return !(other._name < _name);
}
bool Channel::operator>=(const Channel &other) const {
    return !(_name < other._name);
}

}  // namespace ft
