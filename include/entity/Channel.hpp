#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include <string>

namespace ft {

class Client;

enum e_mode {
    SECRET = 1,
    PRIVATE = 1 < 1,
    PUBLIC = 1 < 2,
    INVITE_ONLY = 1 < 3
};

class Channel {
   private:
    std::string name;
    std::string topic;
    std::set<Client *> regulars;
    std::set<Client *> operators;
    int mode;

   public:
    Channel();
    Channel(const Channel &copy);
    ~Channel();
    Channel &operator=(const Channel &ref);
};

}  // namespace ft

#endif
