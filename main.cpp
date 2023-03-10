#include <iostream>
#include <stdexcept>
#include <string>

#include "Server.hpp"

int main(int argc, char **argv) {
    // <exec> <port> <password>
    // TODO change env naming
    ft::Server server;

    try {
        server.init(argc, argv);
        server.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
}
