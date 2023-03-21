#ifndef CHANNELCONTROLLER_HPP
#define CHANNELCONTROLLER_HPP

#include <map>
#include <set>
#include <string>

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

    void updateMode(int mode, Channel *channel);
    void updateMode(int mode, const std::string &channel_name);
    void updateTopic(Client *client, Channel *channel,
                     const std::string &topic);

    void insertClient(Channel *channel, Client *client, bool is_operator);

    void eraseClient(Channel *channel, Client *client);

    void eraseClient(ChannelList &channel_list, Client *client);

    bool hasPermission(Channel *channel, Client *client);
};

}  // namespace ft

#endif
