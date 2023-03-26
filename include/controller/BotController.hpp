#ifndef BOTCONTROLLER_HPP
#define BOTCONTROLLER_HPP

#include <iomanip>
#include <sstream>
#include "core/utility.hpp"

namespace ft {

class BotController {
   public:
    BotController();
    BotController(const BotController &copy);
    ~BotController();

    BotController &operator=(const BotController &ref);

    std::string help();
    std::string hi();
    std::string bye();
    std::string now();
    std::string weather();
};

}

#endif //BOTCONTROLLER_HPP
