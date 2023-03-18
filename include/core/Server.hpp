#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

#include "Parser.hpp"
#include "controller/Executor.hpp"
#include "core/EventHandler.hpp"
#include "core/Socket.hpp"

namespace ft {

struct Env {
    int port;
    std::string password;
    static const int max_client = 500;
    static const int max_channel = 30;

    // method
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
    void handleConnect(int event_idx);
    void handleRead(int event_idx);
    void handleExecute(int event_idx);
    void handleWrite(int event_idx);
    void handleTimeout(int event_idx);
};

}  // namespace ft

#endif /* SERVER_HPP */
