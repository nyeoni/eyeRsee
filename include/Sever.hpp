#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Client.hpp"  // #include "Response.hpp"
#include "EventHandler.hpp"
#include "Socket.hpp"

namespace ft {

namespace max_num {
static const int client = 500;
static const int channel = 30;
}  // namespace max_num

struct Env {
    int port;
    std::string password;

    // method
    void parse(int argc, char **argv);
};

class Server {
   private:
    Env _env;
    ListeningSocket listening_socket;
    ConnectSocket *ConnectSocket;  // new ft::max_num::
    int _listen_fd;
    int _kq_fd;

   public:
    Server();
    Server(const Server &copy);
    ~Server();
    Server &Server::operator=(const Server &ref);

    void init(int argc, char **argv) {
        // init_env -> task

        // 진짜 찐 준비 서버가 준비운동하기
        // listen_fd
        // kq_fd
    }
};
}  // namespace ft

#endif /* SERVER_HPP */
