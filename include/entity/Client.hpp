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
    typedef std::set<Channel *>::iterator channel_list_ieterator;

   private:
    // typedef typename std::set<Channel *>::iterator channel_iterator;
    //    int fd;				// CHECK
    std::string _nickname;  // identifier
    std::string _username;
    std::string _hostname;
    std::string _server;
    std::string _realname;
    ChannelList _channel_list;    // channel list
    ChannelList _i_channel_list;  // invite channel list

   public:
    Client(/* args*/);
    Client(const std::string &nickname);
    Client(const Client &copy);
    ~Client();
    Client &operator=(const Client &ref);

    // getter
    ChannelList getChannelList() const;
    std::string getNickname() const;
    std::string getHostname() const;
    std::string getRealname() const;

    // setter
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setHostname(const std::string &hostname);
    void setServer(const std::string &server);
    void setRealname(const std::string &realname);

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
