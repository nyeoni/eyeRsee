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
    channel_iterator iter = _channels.find(channel->getName());
    if (iter == _channels.end()) return NULL;
    return &(iter->second);
}

Channel *ChannelController::find(const std::string &name) {
    channel_iterator iter = _channels.find(name);
    if (iter == _channels.end()) return NULL;
    return &(iter->second);
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

void ChannelController::updateMode(int mode, Channel *channel) {
    channel->setMode(mode);
}
void ChannelController::updateMode(int mode, const std::string &name) {
    Channel *target = find(name);
    if (target) return target->setMode(mode);
}

void ChannelController::updateTopic(Client *client, Channel *channel,
                                    const std::string &topic) {
    // permission denid - not on channel, not an operator
    // success
    if (!channel->isOnChannel(client)) {
        // error not on channel
        return;
    }
    if (channel->isTopicMode()) {
        if (channel->isOperator(client))
            channel->setTopic(topic);
        else {
            // not an operator
        }
    } else {
        channel->setTopic(topic);
    }
}

void ChannelController::insertClient(Channel *channel, Client *client,
                                     bool is_operator) {
    channel->insertClient(client, is_operator);
}

void ChannelController::eraseClient(Channel *channel, Client *client) {
    channel->eraseClient(client);
    if (channel->getOperators().size() + channel->getRegulars().size() == 0) {
        del(channel);
    }
}

}  // namespace ft
