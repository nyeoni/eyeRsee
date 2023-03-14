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

   private:
    // typedef typename std::set<Channel *>::iterator channel_iterator;
    //    int fd;				// CHECK
    std::string _nickname;  // identifier
    std::string _hostname;
    std::string _realname;
    ChannelList _channel_list;  // channel list

   public:
    Client(/* args*/);
    Client(const std::string &nickname);
    Client(const Client &copy);
    ~Client();
    Client &operator=(const Client &ref);

    // getter
    ChannelList getChannels() const;
    std::string getNickname() const;
    std::string getHostname() const;
    std::string getRealname() const;

    // setter
    void setNickname(const std::string &nickname);
    void setHostname(const std::string &hostname);
    void setRealname(const std::string &realname);

    // update
    void insertChannel(Channel *channel);
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