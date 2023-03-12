#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/event.h>

#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "Response.hpp"  // udata
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

class Server {
   private:
    typedef std::vector<ConnectSocket> sockets;

    Env _env;
    ListenSocket _listen_socket;
    // ConnectSocket *_connect_socket;  // new ft::max_num::
    // new [10]
    // TODO :
    sockets _connect_sockets;

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

        HandlerInfo<sockets> _info;
        // Udata<sockets> _udata;

        EventHandler();

       public:
        EventHandler(Server &server_info);
        ~EventHandler();

        // int init(int fd);
        Event *changeList();
        int monitorEvent();
        void handle(int event_idx);
        void handleAccept(int listen_fd);
        void handleRead(int fd);
        void handleExcute(int fd);
        void handleWrite(UdataWrite &udata);
        // void handleAccept(Udata<sockets> &udata);
        // void handleRead(Udata<sockets> &udata);
        // void handleWrite(Udata<sockets> &udata);
        //  void updateChangeList(int fd);
        //, int filt, int flags, int action;
        void registerEvent(int fd, int action);
        //void
    };
    // EventHandler event_handler;
    // friend class EventHandler;
};

}  // namespace ft

#endif /* SERVER_HPP */
