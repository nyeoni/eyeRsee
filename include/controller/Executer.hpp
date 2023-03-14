#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <string>

#include "controller/ChannelController.hpp"
#include "controller/ClientController.hpp"

namespace ft {

// class ChannelController;
// class ClientController;
class Executer {
 private:
//    ChannelController channel_controller;
    ClientController client_controller;

 public:
    Executer();
    Executer(const Executer &copy);
    ~Executer();
    Executer &operator=(const Executer &ref);

    void connect(Client *client);

    // method
    // TODO : findClient(int fd);  // kevent - fd, udata  -> recv -> JOIN #abc
//    void deleteClient(std::string nickname);
//    void joinClient(std::string nickname, std::string channel_name);
};

// Executer -> Server data update
// recv command 수행 PART/QUIT

// join, part, mode, ... cmds
// switch(cmd)
// case: join
// Executer.joinClient()

}  // namespace ft

#endif