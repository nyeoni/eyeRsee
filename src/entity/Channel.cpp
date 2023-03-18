#include "entity/Channel.hpp"

#include "core/Type.hpp"

namespace ft {

Channel::Channel() : _mode(0) {}
Channel::Channel(const std::string &name) : _name(name) {}
Channel::Channel(const Channel &copy) {
    if (*this != copy) {
        *this = copy;
    }
}
Channel::~Channel() {}
Channel &Channel::operator=(const Channel &ref) {
    _name = ref._name;
    _topic = ref._topic;
    _regulars = ref._regulars;
    _operators = ref._operators;
    _mode = ref._mode;

    return (*this);
}

std::string Channel::getName() const { return _name; }
std::string Channel::getTopic() const { return _topic; }
Channel::ClientList Channel::getRegulars() const { return _regulars; }
Channel::ClientList Channel::getOperators() const { return _operators; }
int Channel::getMode() const { return _mode; }

void Channel::setName(const std::string &name) { _name = name; }
void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setMode(int mode) {
    if (mode % 2)
        _mode |= (1 << (mode - 1));  // +flag
    else
        _mode &= ~(1 << (mode));  // -flag
}

// update
void Channel::insertClient(Client *client, bool is_operator) {
    if (is_operator)
        _operators.insert(client);
    else
        _regulars.insert(client);
}

void Channel::eraseClient(Client *client) {
    if (isOperator(client)) {
        _operators.erase(client);
    } else {
        _regulars.erase(client);
    }
}

bool Channel::isInviteMode() { return (_mode & (INVITE_ONLY_T - 1)); }
bool Channel::isTopicMode() { return (_mode & (TOPIC_PRIV_T - 1)); }
bool Channel::isBanMode() { return (_mode & (BAN_T - 1)); }

bool Channel::isOnChannel(Client *client) {
    return (isOperator(client) || isRegular(client));
}
bool Channel::isOperator(Client *client) {
    return _operators.find(client) != _operators.end() ? true : false;
}
bool Channel::isRegular(Client *client) {
    return _regulars.find(client) != _regulars.end() ? true : false;
}

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
