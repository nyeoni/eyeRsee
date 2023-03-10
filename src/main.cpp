#include <exception>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    // <exec> <port> <password>
    // TODO change env naming
    Server server;

    try {
        server.init();
        server.run()
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }
}

void init_env(Env &env, int argc, char **argv) {
    if (argc != 3) throw std::exception("Invalid Argument error");
    // TODO chaj port
    env.password = argv[2];
}