#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include <string>

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

    const std::string &getName() const;
    const std::string &getTopic() const;
    const ClientList &getRegulars() const;
    const ClientList &getOperators() const;
    int getMode() const;

    void setName(const std::string &name);
    void setTopic(const std::string &topic);
    void setMode(int mode);

    void clearMode();

    // update
    void insertOperator(Client *client);
    void insertRegular(Client *client);

    void eraseOperator(Client *client);
    void eraseRegular(Client *client);

    bool operator==(const Channel &other) const;
    bool operator!=(const Channel &other) const;
    bool operator<(const Channel &other) const;
    bool operator>(const Channel &other) const;
    bool operator<=(const Channel &other) const;
    bool operator>=(const Channel &other) const;
};

}  // namespace ft

#endif
