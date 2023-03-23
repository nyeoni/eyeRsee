#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <set>
#include <string>

#include "core/Socket.hpp"

namespace ft {

class Channel;

class Client : public ConnectSocket {
   public:
    typedef std::set<Channel *> ChannelList;
    typedef std::set<Channel *>::iterator channel_list_iterator;

   private:
    // typedef typename std::set<Channel *>::iterator channel_iterator;
    ChannelList _channel_list;    // channel list
    ChannelList _i_channel_list;  // invite channel list

   public:
    Client(/* args*/);
    Client(const Client &copy);
    ~Client();
    Client &operator=(const Client &ref);

    // getter
    const ChannelList &getChannelList() const;

    // update
    void insertChannel(Channel *channel);
    void insertInviteChannel(Channel *channel);

    void eraseChannel(Channel *channel);

    // compare operators
    bool operator==(const Client &other) const;
    bool operator!=(const Client &other) const;
    bool operator<(const Client &other) const;
    bool operator>(const Client &other) const;
    bool operator<=(const Client &other) const;
    bool operator>=(const Client &other) const;
};

}  // namespace ft

#endif
