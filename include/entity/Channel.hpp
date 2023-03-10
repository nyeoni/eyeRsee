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
    void setMode(int add_mode, int del_mode);

    bool operator==(const Channel &other) const;
    bool operator!=(const Channel &other) const;
    bool operator<(const Channel &other) const;
    bool operator>(const Channel &other) const;
    bool operator<=(const Channel &other) const;
    bool operator>=(const Channel &other) const;
};

}  // namespace ft

#endif
