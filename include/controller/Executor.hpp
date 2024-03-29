#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <vector>

#include "controller/BotController.hpp"
#include "controller/ChannelController.hpp"
#include "controller/ClientController.hpp"
#include "core/Command.hpp"

namespace ft {
class Client;

class Executor {
   private:
    std::set<Client *> _client_list;

    ChannelController channel_controller;
    ClientController client_controller;
    BotController bot_controller;

    typedef std::set<Channel *> ChannelList;
    typedef std::set<Client *> ClientList;
    typedef std::set<Channel *>::iterator channel_list_iterator;
    typedef std::set<Client *>::iterator client_list_iterator;

   public:
    Executor();
    Executor(const Executor &copy);
    ~Executor();
    Executor &operator=(const Executor &ref);

    const std::set<Client *> &getClientList() const;
    void clearClientList();

    Client *accept(int fd);
    bool connect(Client *client, std::string password);
    void execute(Command *command, Client *client, std::string password);
    void execute(Command *command, Client *client);
    Channel *createChannel(std::string channel_name);

    int updateClientStatus(int fd, Client *client, e_status status);
    void deleteClient(Client *client);

   private:
    void nick(int fd, params *params);
    void nick(Client *new_client, params *params);
    void user(Client *new_client, params *params);
    void pass(Client *new_client, params *params, std::string server_password);

    void join(Client *client, params *params);
    void part(Client *client, params *params);
    void mode(Client *client, params *params);
    void topic(Client *client, params *params);
    void invite(Client *invitor, params *params);
    void kick(Client *kicker, params *params);
    void quit(Client *client, params *params);
    void privmsg(Client *client, params *params);
    void notice(Client *client, params *params);
    void pong(Client *client);

    void bot(Client *client, params *params);

    void broadcast(Channel *channel, const std::string &msg,
                   Client *excluded = NULL);
    void broadcast(const ChannelList &channel_list, const std::string &msg);
};
}  // namespace ft

#endif
