#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <vector>

#include "controller/ChannelController.hpp"
#include "controller/ClientController.hpp"
// #include "core/Type.hpp"  // ChannelController.hpp 로 이사
#include "core/Udata.hpp"

namespace ft {
class Client;

class Executor {
   public:
    // typedef std::vector<std::string> std::vector<std::string>;
    typedef std::vector<std::string>::iterator vector_iterator;
    // typedef std::set<Client *> ClientList;

   private:
    ChannelController channel_controller;
    ClientController client_controller;

   public:
    Executor(/* args*/);
    Executor(const Executor &copy);
    ~Executor();
    Executor &operator=(const Executor &ref);

    // method
    Client *creatClient(int fd);
    void part(Client *client, params *params);
    // int joinClient(std::string nickname, std::string channel_name);

    void join(Client *client, params *params);
    void mode(Client *client, params *params);
    void topic(Client *client, params *params);
    void invite(Client *invitor, params *params);

    void kick(Client *kicker, params *params);
    void pass(Client *new_client, params *params, std::string server_password);
    void user(Client *new_client, params *params);
    void nick(Client *new_client, params *params);
    void nick(int fd, params *params);

    void quit(Client *client, params *params);

    void privmsg(Client *client, params *params);
};
}  // namespace ft

#endif
