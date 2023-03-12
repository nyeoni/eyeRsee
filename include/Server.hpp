#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/event.h>

#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "EventHandler.hpp"
#include "Response.hpp"
#include "Socket.hpp"

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
    typedef std::vector<ConnectSocket> SocketList;

    Env _env;
    ListenSocket _listen_socket;
    // ConnectSocket *_connect_socket;  // new ft::max_num::
    // new [10]
    // TODO :
    SocketList _socket_list;

   public:
    Server();
    ~Server();

    // main method
    void init(int argc, char **argv);
    void run();

    // handler methods
    void handleAccept();
    void handleRead(int event_idx);
    void handleExcute(int event_idx);
    void handleWrite(int event_idx);
};

}  // namespace ft

#endif /* SERVER_HPP */
