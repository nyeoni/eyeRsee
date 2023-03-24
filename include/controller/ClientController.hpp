#ifndef CLIENTCONTROLLER_HPP
#define CLIENTCONTROLLER_HPP

#include <map>
#include <set>
#include <string>
#include <utility>

#include "core/Type.hpp"

namespace ft {

class Client;
class Channel;

class ClientController {
   public:
    typedef std::map<int, Client> Clients;
    typedef Clients::iterator client_iterator;
    typedef std::pair<client_iterator, bool> pair;
    typedef std::set<Client*> ClientList;

    typedef std::set<Channel*> ChannelList;
    typedef std::set<Channel*>::iterator channel_list_iterator;

   private:
    Clients _clients;

   public:
    ClientController(/* args*/);
    ClientController(const ClientController& copy);
    ~ClientController();
    ClientController& operator=(const ClientController& ref);

    Client* find(const int fd);
    Client* find(const Client* client);
    Client* find(const std::string& nickname);

    void findInSet(ChannelList& channel_list, Client* client);

    Client* insert(int fd);

    void erase(const Client* client);
    void erase(const std::string& nickname);
    void erase(int fd);

    void updateClientStatus(int fd, Client* client, e_status status);

    //void updateNickname(int fd, const std::string& nickname);
    void updateNickname(Client* client, const std::string& nickname);

    void insertChannel(Client* client, Channel* channel);
    void insertInviteChannel(Client* client, Channel* channel);

    void eraseChannel(Client* client, Channel* channel);

    // make client list to vector<string>
    std::vector<std::string> clientToString(ClientList client_list);
};

}  // namespace ft

#endif
