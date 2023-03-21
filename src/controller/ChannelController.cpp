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

Channel *ChannelController::find(const std::string &channel_name) {
    channel_iterator iter = _channels.find(channel_name);
    if (iter == _channels.end()) return NULL;
    return &(iter->second);
}

void ChannelController::findInSet(ClientList &client_list, Channel *channel) {
    const ClientList &ops = channel->getOperators();
    const ClientList &regs = channel->getRegulars();

    client_list.insert(ops.begin(), ops.end());
    client_list.insert(regs.begin(), regs.end());
}

void ChannelController::erase(const Channel *channel) {  // const
    _channels.erase(channel->getName());
}

void ChannelController::erase(const std::string &channel_name) {
    _channels.erase(channel_name);
}

void ChannelController::updateMode(int mode, Channel *channel) {
    channel->setMode(mode);
}
void ChannelController::updateMode(int mode, const std::string &channel_name) {
    Channel *target = find(channel_name);
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

bool ChannelController::hasPermission(Channel *channel, Client *client) {
    if (channel->isOperator(client)) {
        // client->insertIChannel();
        return true;
    } else if (channel->isRegular(client)) {
        // #name You must be a channel op or higher to send an invite.
    } else {
        // you are not on channel
    }
    // channel->insertClient(client, true);
    return false;
}

/**
 * @brief insert Client to Channel's _clientList
 */
void ChannelController::insertClient(Channel *channel, Client *client,
                                     bool is_operator) {
    channel->insertClient(client, is_operator);
}

/**
 * @brief erase Client to Channel's _clientList
 */
void ChannelController::eraseClient(Channel *channel, Client *client) {
    channel->eraseClient(client);
    if (channel->getOperators().size() + channel->getRegulars().size() == 0) {
        erase(channel);
    }
}

/**
 * @brief Clear Client to _clientList on all channels
 */
void ChannelController::eraseClient(ChannelList &channel_list, Client *client) {
    channel_list_iterator iter = channel_list.begin();
    for (; iter != channel_list.end(); ++iter) {
        eraseClient(*iter, client);
    }
}

Channel *ChannelController::insert(const std::string &channel_name) {
    Channel *new_channel;
    pair p = _channels.insert(std::make_pair(channel_name, Channel()));
    new_channel = &(p.first->second);
    new_channel->setName(channel_name);
    new_channel->clearMode();
    return new_channel;
}

}  // namespace ft
