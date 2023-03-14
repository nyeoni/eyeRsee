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
void Channel::updateClientList(Client *client, bool is_operator,
                               bool is_insert) {
    if (is_operator) {
        if (is_insert)
            _operators.insert(client);
        else
            _operators.erase(client);
    } else {
        if (is_insert)
            _regulars.insert(client);
        else
            _regulars.erase(client);
    }
}

bool Channel::is_invite_mode() { return (_mode & (INVITE_ONLY_T - 1)); }
bool Channel::is_topic_mode() { return (_mode & (TOPIC_PRIV_T - 1)); }
bool Channel::is_ban_mode() { return (_mode & (BAN_T - 1)); }

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
