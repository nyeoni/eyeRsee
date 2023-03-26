#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

#include "controller/Executor.hpp"
#include "core/Parser.hpp"
#include "core/Socket.hpp"
#include "handler/EventHandler.hpp"

namespace ft {

struct Env {
    int port;
    std::string password;
    static const int max_client = 500;
    static const int max_channel = 30;

    void parse(int argc, char **argv);
};

class Server : public EventHandler {
   private:
    ListenSocket _listen_socket;

    Env _env;
    Parser _parser;
    Executor _executor;

   public:
    Server();
    ~Server();

    // main method
    void init(int argc, char **argv);
    void run();

    // handler methods
    void handleAccept();
    void handleRead(int event_idx);
    void handleExecute(int event_idx);
    void handleTimer(int event_idx);

    // garbageCollector methods
    void handleClose();

   private:
    int parse(int fd, Client *client);
    void connect(int fd, Client *client);
    void reserve();
    int response(int fd, std::string &send_buf);

    void destroyCommands(std::queue<Command *> &commands);
};

}  // namespace ft

#endif /* SERVER_HPP */
