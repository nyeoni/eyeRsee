#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include <string>

// #include <entity/Client.hpp>

namespace ft {

class Client;

enum e_type {
    SECRET = 1,
    PRIVATE = 1 < 1,
    PUBLIC = 1 < 2,
    INVITE_ONLY = 1 < 3,
    T = 1 < 4
};

class Channel {
   public:
    typedef std::set<Client *> ClientList;
    typedef std::set<Client *>::iterator client_list_iterator;

   private:
    std::string _name;
    std::string _topic;
    ClientList _regulars;
    ClientList _operators;
    int _mode;

   public:
    Channel();
    Channel(const std::string &name);
    Channel(const Channel &copy);
    ~Channel();
    Channel &operator=(const Channel &ref);

    std::string getName() const;
    std::string getTopic() const;
    ClientList getRegulars() const;
    ClientList getOperators() const;
    int getMode() const;

    void setName(const std::string &name);
    void setTopic(const std::string &topic);
    // void setRegulars(const Client *client);
    // void setOperators(const Client *client);
    void setMode(int mode);

    // update
    void insertClient(Client *client, bool is_operator);
    void eraseClient(Client *client);
    // void updateClientList(Client *client, bool is_insert);

    bool isOnChannel(Client *client);
    bool isOperator(Client *client);
    bool isRegular(Client *client);

    bool isInviteMode();
    bool isTopicMode();
    bool isBanMode();

    bool operator==(const Channel &other) const;
    bool operator!=(const Channel &other) const;
    bool operator<(const Channel &other) const;
    bool operator>(const Channel &other) const;
    bool operator<=(const Channel &other) const;
    bool operator>=(const Channel &other) const;
};

}  // namespace ft

#endif
