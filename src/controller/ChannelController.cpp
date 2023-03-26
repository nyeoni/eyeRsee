#include "controller/ChannelController.hpp"

#include <utility>

#include "core/Type.hpp"
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

Channel *ChannelController::insert(const std::string &channel_name) {
    Channel *new_channel;
    pair p = _channels.insert(std::make_pair(channel_name, Channel()));
    new_channel = &(p.first->second);
    new_channel->setName(channel_name);
    return new_channel;
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

/**
 * @return true - attemping to to update to the different mode
 * @return false - attemping to update to the same mode
 * @note mode == OPER_F || mode == OPER_T => client is not NULL
 * TODO int mode -> e_mode mode
 */
bool ChannelController::updateMode(int mode, Channel *channel, Client *client) {
    if (mode == OPER_F)
        return updateRole(channel, client, REGULAR);
    else if (mode == OPER_T)
        return updateRole(channel, client, OPERATOR);

    if ((mode == INVITE_ONLY_F && !isInviteMode(channel)) ||
        (mode == INVITE_ONLY_T && isInviteMode(channel))) {
        return false;
    }

    if (mode == TOPIC_PRIV_F && !isTopicMode(channel) ||
        mode == TOPIC_PRIV_T && isTopicMode(channel)) {
        return false;
    }

    channel->setMode(mode);
    return true;
}

void ChannelController::updateTopic(Channel *channel, Client *client,
                                    const std::string &topic) {
    channel->setTopic(topic);
}

const std::set<Client *> &ChannelController::getOperators(
    const Channel *channel) const {
    return channel->getOperators();
}

const std::set<Client *> &ChannelController::getRegulars(
    const Channel *channel) const {
    return channel->getRegulars();
}

bool ChannelController::isOnChannel(Channel *channel, Client *client) {
    return (isOperator(channel, client) || isRegular(channel, client));
}

bool ChannelController::isOperator(Channel *channel, Client *client) {
    std::set<Client *> operators = channel->getOperators();

    return operators.find(client) != operators.end() ? true : false;
}

bool ChannelController::isRegular(Channel *channel, Client *client) {
    std::set<Client *> regulars = channel->getRegulars();

    return regulars.find(client) != regulars.end() ? true : false;
}

void ChannelController::insertOperator(Channel *channel, Client *client) {
    channel->insertOperator(client);
}

void ChannelController::insertRegular(Channel *channel, Client *client) {
    channel->insertRegular(client);
}

void ChannelController::insertInvitedClient(Channel *channel, Client *client) {
    channel->insertInvitedClient(client);
}

/**
 * @brief erase Client to Channel's _clientList
 */
void ChannelController::eraseClient(Channel *channel, Client *client) {
    isOperator(channel, client) ? channel->eraseOperator(client)
                                : channel->eraseRegular(client);
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

bool ChannelController::isInviteMode(const Channel *channel) {
    // return (channel->getMode() & (INVITE_ONLY_T - 1));
    return (channel->getMode() & (1 << (INVITE_ONLY_T / 2)));
}

bool ChannelController::isTopicMode(const Channel *channel) {
    // return (channel->getMode() & (TOPIC_PRIV_T - 1));
    return (channel->getMode() & (1 << (TOPIC_PRIV_T / 2)));
}

bool ChannelController::isBanMode(const Channel *channel) {
    // return (channel->getMode() & (BAN_T - 1));
    return (channel->getMode() & (1 << (BAN_T / 2)));
}

// private functions
/**
 * @brief update Channel::_operators and Channel::_regulars
 *
 * @param role - role to update (operator || regular)
 *
 * @return true - attemping to update to the different mode
 * @return false - attemping to update to the same mode
 */
bool ChannelController::updateRole(Channel *channel, Client *client,
                                   e_role role) {
    // update to regular (operator -> regular)
    if (role == REGULAR && isOperator(channel, client)) {
        insertRegular(channel, client);
        eraseOperator(channel, client);
        return true;
    }

    // update to operaor (regular -> operator)
    if (role == OPERATOR && isRegular(channel, client)) {
        insertOperator(channel, client);
        eraseRegular(channel, client);
        return true;
    }

    // there are no change
    return false;
}

void ChannelController::eraseOperator(Channel *channel, Client *client) {
    channel->eraseOperator(client);
}

void ChannelController::eraseRegular(Channel *channel, Client *client) {
    channel->eraseRegular(client);
}

}  // namespace ft
