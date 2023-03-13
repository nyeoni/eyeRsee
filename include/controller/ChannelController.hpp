#ifndef CHANNELCONTROLLER_HPP
#define CHANNELCONTROLLER_HPP

#include <set>

namespace ft {

class Channel;

class ChannelController {
   public:
    typedef std::set<Channel> Channels;
    // typedef std::set<Channel *> ChannelList;
    typedef Channels::iterator channel_iterator;

   private:
    Channels _channels;

    ChannelController(/* args*/);
    ChannelController(const ChannelController &copy);
    ~ChannelController();
    ChannelController &operator=(const ChannelController &ref);

    channel_iterator find(const Channel *channel) const;
    channel_iterator find(const std::string &name) const;
    //Channel *find(const Channel *channel) const;
    //Channel *find(const std::string &name) const;

    void add(const Channel *channel);
    void add(const std::string &name);

    void del(Channel *channel);         // const
    void del(const std::string &name);  // const

    void mod(int mode, Channel *channel);
    void mod(int mode, const std::string &name);
    void modTopic(Channel *channel, const std::string &name);
};

}  // namespace ft

#endif
