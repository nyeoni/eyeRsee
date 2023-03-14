#ifndef CLIENTCONTROLLER_HPP
#define CLIENTCONTROLLER_HPP

#include <map>
#include <string>
#include <utility>

namespace ft {

class Client;

class ClientController {
   public:
    typedef std::map<int, Client> Clients;
    typedef Clients::iterator client_iterator;

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

    void create(const Client* client);
    void create(int fd, const Client* client);

    void del(const Client* client);
    void del(const std::string& nickname);
    void del(int fd);

    void update(int fd, const std::string& nickname);
    void update(Client* client, const std::string& nickname);

    // find duplicated nickname

    // ClinetController -> updateChannel
    //

    // ChannelController -> updateClient
};

}  // namespace ft

#endif
