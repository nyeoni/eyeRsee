#include "controller/ChannelController.hpp"

#include "entity/Channel.hpp"

namespace ft {
ChannelController::ChannelController() {}

ChannelController::ChannelController(const ChannelController &copy) {
    *this = copy;
}

ChannelController::~ChannelController() {}

ChannelController &ChannelController::operator=(const ChannelController &ref) {
    // if (*this != ref) {
    // }
    return (*this);
}

ChannelController::channel_iterator ChannelController::find(
    const Channel *channel) const {
    return (_channels.find(*channel));
}

ChannelController::channel_iterator ChannelController::find(
    const std::string &name) const {
    Channel tmp(name);
    return (_channels.find(tmp));
}

// Channel *ChannelController::find(const Channel *x) const {
//     // channel_iterator iter = _channels.find(*channel);
//     // return static_cast<Channel *>(&(*(iter)));
//     return (Channel *)&(*(_channels.find(*x)));
// }

// Channel *ChannelController::find(const std::string &name) const {
//     Channel tmp(name);
//     return (Channel *)&(*(_channels.find(tmp)));
// }

void ChannelController::add(const Channel *channel) {
    _channels.insert(*channel);
}
void ChannelController::add(const std::string &name) {
    Channel tmp(name);
    _channels.insert(tmp);
}

void ChannelController::del(Channel *channel) {  // const
    _channels.erase(*channel);
}
void ChannelController::del(const std::string &name) {
    Channel tmp(name);
    _channels.erase(name);
}

void ChannelController::modTopic(Channel *channel, const std::string &topic) {
    channel->setTopic(topic);
}
void ChannelController::mod(int mode, Channel *channel) {
    switch (mode) {
        case T:
            channel->setMode(mode, 0);
            break;
        //case DT:
        //    channel->setMode(0, mode);
        //    break;
        default:
            break;
    }
}
void ChannelController::mod(int mode, const std::string &name) {}

}  // namespace ft
