#ifndef CHANNELCONTROLLER_HPP
#define CHANNELCONTROLLER_HPP

#include <map>

namespace ft {

class Channel;
class Client;

class ChannelController {
   public:
    // std::string & 되나
    typedef std::map<std::string, Channel> Channels;
    // TODO Consider client_iterator || Client *
    typedef Channels::iterator channel_iterator;
    // typedef std::pair<client_iterator, bool>;

   private:
    Channels _channels;

   public:
    ChannelController(/* args*/);
    ChannelController(const ChannelController &copy);
    ~ChannelController();
    ChannelController &operator=(const ChannelController &ref);

    Channel *find(const Channel *channel);
    Channel *find(const std::string &name);

    void create(const Channel *channel);
    void create(const std::string &name);

    void del(const Channel *channel);
    void del(const std::string &name);

    void updateMode(int mode, Channel *channel);
    void updateMode(int mode, const std::string &name);
    void updateTopic(Client *client, Channel *channel, const std::string &topic);

    /**
     * @brief insert Client to Channel's _clientList
     */
    void insertClient(Channel *channel, Client *client, bool is_operator);

    /**
     * @brief erase Client to Channel's _clientList
     */
    void eraseClient(Channel *channel, Client *client);

    bool hasPermission(Channel *channel, Client *client);
};

}  // namespace ft

#endif
