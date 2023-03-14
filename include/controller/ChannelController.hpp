#ifndef CHANNELCONTROLLER_HPP
#define CHANNELCONTROLLER_HPP

#include <map>

namespace ft {

class Channel;

class ChannelController {
   public:
    typedef std::string &Key;
    typedef Channel Val;
    typedef std::map<Key, Val> Channels;
    // TODO Consider client_iterator || Client *
    typedef Channels::iterator client_iterator;

   private:
    Channels _channels;

   public:
    ChannelController(/* args*/);
    ChannelController(const ChannelController &copy);
    ~ChannelController();
    ChannelController &operator=(const ChannelController &ref);

    Channel *find(const Channel *channel) const;
    Channel *find(const std::string &name) const;

    void create(const Channel *channel);
    void create(const std::string &name);

    void del(const Channel *channel);
    void del(const std::string &name);

    void mod(int mode, Channel *channel);
    void mod(int mode, const std::string &name);
    void modTopic(Channel *channel, const std::string &name);
};

}  // namespace ft

#endif
