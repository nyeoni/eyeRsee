#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <set>
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
    typedef std::vector<std::string> CmdLine;
    typedef std::vector<std::string>::iterator cmd_iterator;
    typedef std::set<Client *> ClientList;

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
    void part(int fd, CmdLine channels);
    // int joinClient(std::string nickname, std::string channel_name);

    void join(int fd, CmdLine cmd_line);
    void mode(int fd, std::string channel_name,
              e_mode mode);  // std::string info
    void topic(int fd, std::string channel_name,
               std::string topic);  // fd -> client ...
    void invite(int fd, std::string nickname, std::string channel_name);

    void kick(int fd, std::string channel_name, std::string nickname,
              std::string comment);
    void pass(Client *new_client, std::string password,
              std::string server_password);
    void user(Client *new_client, std::string username, std::string hostname,
              std::string server, std::string realname);
    void nick(Client *new_client, std::string nickname);
    void nick(int fd, std::string nickname);

    void quit(int fd, std::string msg);

    void privmsg(Client *client, CmdLine receivers, std::string msg);

   private:
    void broadcast(Channel *channel, std::string msg = "",
                   Client *client = NULL);
    void broadcast(Client *client, std::string msg = "");

    ClientList findReceivers(Client *client);
};
}  // namespace ft

#endif
