#include "entity/Channel.hpp"

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
void Channel::setMode(int add_mode , int del_mode) {
    // TODO : flag bit operate
    _mode = add_mode;
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
