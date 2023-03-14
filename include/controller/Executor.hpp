#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <vector>

#include "controller/ChannelController.hpp"
#include "controller/ClientController.hpp"
#include "core/Type.hpp" // ChannelController.hpp 로 이사

namespace ft {

class Executor {
   public:
    typedef std::vector<std::string> CmdLine;
    typedef std::vector<std::string>::iterator cmd_iterator;

   private:
    ChannelController channel_controller;
    ClientController client_controller;

   public:
    Executor(/* args*/);
    Executor(const Executor &copy);
    ~Executor();
    Executor &operator=(const Executor &ref);

    // method
    void part(int fd, CmdLine channels);
    // void joinClient(std::string nickname, std::string channel_name);

    void join(int fd, CmdLine cmd_line);
    void mode(int fd, std::string channel, e_mode mode); // std::string info

    void pass(Client *client);
};

// Executor -> Server data update
// recv command 수행 PART/QUIT

// join, part, mode, ... cmds
// switch(cmd)
// case: join
// Executor.joinClient()

}  // namespace ft

#endif
