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

    if (channel->getMode() == mode) return false;

    channel->setMode(mode);
    return true;
}

void ChannelController::updateTopic(Channel *channel, Client *client,
                                    const std::string &topic) {
    // there are no difference from previous topic
    if (channel->getTopic() == topic) return;

    channel->setTopic(topic);
}

/**
 * @brief update Channel::_operators and Channel::_regulars
 *
 * @param role - role to update (operator || regular)
 *
 * @return true - attemping to to update to the different mode
 * @return false - attemping to update to the same mode
 */
bool ChannelController::updateRole(Channel *channel, Client *client,
                                   e_role role) {
    // update to regular (operator -> regular)
    if (role == REGULAR && isOperator(channel, client) == true) {
        insertRegular(channel, client);
        eraseOperator(channel, client);
        return true;
    }

    // update to operator (regular -> operator)
    if (role == OPERATOR && isOperator(channel, client) == false) {
        insertOperator(channel, client);
        eraseRegular(channel, client);
        return true;
    }

    // there are no change
    return false;
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

/**
 * @brief insert Client to Channel's _clientList
 */
void ChannelController::insertClient(Channel *channel, Client *client,
                                     e_role role) {
    role == OPERATOR ? channel->insertOperator(client)
                     : channel->insertRegular(client);
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

Channel *ChannelController::insert(const std::string &channel_name) {
    Channel *new_channel;
    pair p = _channels.insert(std::make_pair(channel_name, Channel()));
    new_channel = &(p.first->second);
    new_channel->setName(channel_name);
    new_channel->clearMode();
    return new_channel;
}

bool ChannelController::isInviteMode(Channel *channel) {
    return (channel->getMode() & (INVITE_ONLY_T - 1));
}

bool ChannelController::isTopicMode(Channel *channel) {
    return (channel->getMode() & (TOPIC_PRIV_T - 1));
}

bool ChannelController::isBanMode(Channel *channel) {
    return (channel->getMode() & (BAN_T - 1));
}

// private functions
void ChannelController::insertOperator(Channel *channel, Client *client) {
    channel->insertOperator(client);
}

void ChannelController::insertRegular(Channel *channel, Client *client) {
    channel->insertRegular(client);
}
void ChannelController::eraseOperator(Channel *channel, Client *client) {
    channel->eraseOperator(client);
}

void ChannelController::eraseRegular(Channel *channel, Client *client) {
    channel->eraseRegular(client);
}

}  // namespace ft
