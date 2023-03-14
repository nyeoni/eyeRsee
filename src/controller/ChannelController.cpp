#include "controller/ChannelController.hpp"

#include <utility>

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

Channel *ChannelController::find(const Channel *channel) {
    return &(_channels.find(channel->getName())->second);
}

Channel *ChannelController::find(const std::string &name) {
    return &(_channels.find(name)->second);
}

void ChannelController::create(const Channel *channel) {
    _channels.insert(std::make_pair(channel->getName(), *channel));
}
void ChannelController::create(const std::string &name) {
    Channel new_channel(name);  // TODO : init mode
    _channels.insert(std::make_pair(name, new_channel));
}

void ChannelController::del(const Channel *channel) {  // const
    _channels.erase(channel->getName());
}

void ChannelController::del(const std::string &name) { _channels.erase(name); }

void ChannelController::update(int mode, Channel *channel) {
    channel->setMode(mode);
}
void ChannelController::update(int mode, const std::string &name) {
    find(name)->setMode(mode);
}
void ChannelController::updateTopic(Channel *channel,
                                    const std::string &topic) {
    channel->setTopic(topic);
}

void ChannelController::updateChannel(Channel *channel, Client *client,
                                      bool is_operator, bool is_insert) {
    channel->updateClientList(client, is_operator, is_insert);
}

}  // namespace ft
