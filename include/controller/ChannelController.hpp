#ifndef CHANNELCONTROLLER_HPP
#define CHANNELCONTROLLER_HPP

#include <map>
#include <set>
#include <string>

#include "core/Type.hpp"

namespace ft {

class Channel;
class Client;

class ChannelController {
   public:
    // std::string & 되나
    typedef std::map<std::string, Channel> Channels;
    // TODO Consider client_iterator || Client *
    typedef Channels::iterator channel_iterator;
    typedef std::pair<channel_iterator, bool> pair;

    typedef std::set<Channel *> ChannelList;
    typedef std::set<Channel *>::iterator channel_list_iterator;
    typedef std::set<Client *> ClientList;
    typedef std::set<Client *>::iterator client_list_iterator;

   private:
    Channels _channels;

   public:
    ChannelController(/* args*/);
    ChannelController(const ChannelController &copy);
    ~ChannelController();
    ChannelController &operator=(const ChannelController &ref);

    Channel *insert(const std::string &channel_name);

    Channel *find(const Channel *channel);
    Channel *find(const std::string &channel_name);

    void findInSet(ClientList &client_list, Channel *channel);

    void erase(const Channel *channel);
    void erase(const std::string &channel_name);

    bool updateMode(int mode, Channel *channel, Client *Client = NULL);
    void updateTopic(Channel *channel, Client *client,
                     const std::string &topic);

    void insertOperator(Channel *channel, Client *client);
    void insertRegular(Channel *channel, Client *client);
    void insertInvitedClient(Channel *channel, Client *client);

    void eraseClient(Channel *channel, Client *client);
    void eraseClient(ChannelList &channel_list, Client *client);

    const std::set<Client *> &getOperators(const Channel *channel) const;
    const std::set<Client *> &getRegulars(const Channel *channel) const;

    bool isOnChannel(Channel *channel, Client *client);
    bool isOperator(Channel *channel, Client *client);
    bool isRegular(Channel *channel, Client *client);

    bool isInviteMode(const Channel *channel);
    bool isTopicMode(const Channel *channel);
    bool isBanMode(const Channel *channel);

   private:
    bool updateRole(Channel *channel, Client *client, e_role role);

    void eraseOperator(Channel *channel, Client *client);
    void eraseRegular(Channel *channel, Client *client);
};

}  // namespace ft

#endif
