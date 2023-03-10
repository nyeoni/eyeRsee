#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/event.h>

#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"  // #include "Response.hpp"
#include "Socket.hpp"

namespace ft {

struct Env {
    int port;
    std::string password;
    const int max_client = 500;
    const int max_channel = 30;

    // method
    void parse(int argc, char **argv);
};

class Server {
   private:
    Env _env;
    ListenSocket _listen_socket;
    ConnectSocket *_connect_socket;  // new ft::max_num::

   public:
    Server();
    ~Server();

    // main method
    void init(int argc, char **argv);
    void run();

    // command control method

   private:
    class EventHandler {
       private:
        typedef struct kevent Event;
        typedef std::vector<Event> EventList;

        // TODO max_event 생성자에서 처리해줄지
        static const int _max_event = 20;
        int _kq_fd;
        int _change_cnt;

        Event _ev_list[_max_event];
        EventList _change_list;

       public:
        EventHandler();
        ~EventHandler();

        // int init(int fd);
        void handle(int event_idx) {}

        void updateChangeList(int fd);
        int monitorEvent();
    };
};

}  // namespace ft

#endif /* SERVER_HPP */
