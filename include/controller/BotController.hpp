#ifndef BOTCONTROLLER_HPP
#define BOTCONTROLLER_HPP


// delete
#include <iostream>

namespace ft {

class BotController {
   public:
    BotController() {}
    BotController(const BotController &copy) {}
    ~BotController() {}

    BotController &operator=(const BotController &ref) {}

    void help() {
        std::cout << "Bot: help" << std::endl;

    }
    void hi() {

    }
    void now() {
        std::cout << "Bot: time" << std::endl;
        time_t now = time(NULL);

    }

};

}

#endif //BOTCONTROLLER_HPP
