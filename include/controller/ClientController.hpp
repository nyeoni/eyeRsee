/**
 * @file ClientController.cpp
 * @author hannkim (hannkim@student.42seoul.kr)
 * @brief controller of Client
 * @version 0.1
 * @date 2023-03-13
 *
 * @copyright Copyright (c) 2023
 * @note delete, find, add client to Client's channels
 */
#ifndef CLIENTCONTROLLER_HPP
#define CLIENTCONTROLLER_HPP

#include <set>
#include <string>

namespace ft {

class Client;

class ClientController {
   public:
    typedef std::set<Client> Clients;
    // TODO Consider client_iterator || Client *
    typedef typename std::set<Client>::iterator client_iterator;

   private:
    Clients _clients;

   public:
    ClientController(/* args*/);
    ClientController(const ClientController& copy);
    ~ClientController();
    ClientController& operator=(const ClientController& ref);

    // Client* findClient(std::string nickname);
    // void deleteClient(std::string nickname);
    // void addClient(Client client);

    // Client* findClient(Client* client);
    client_iterator findClient(Client* client);
    void deleteClient(Client* client);
    void addClient(Client* client);

    // TODO
    client_iterator findClient(const std::string& nickname);
    void deleteClient(const std::string& nickname);
    void addClient(const std::string& nickname);
};

}  // namespace ft

#endif
