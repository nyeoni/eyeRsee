#include <iostream>
#include <stdexcept>
#include <string>

#include "core/Server.hpp"

int main(int argc, char **argv) {
    // <exec> <port> <password>
    ft::Server server;

    try {
        server.init(argc, argv);
        server.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
