#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/event.h>

#include <vector>

namespace ft {

enum e_event {
    ACCEPT,
    READ,
    READ_MORE,
    WRITE,
    WRITE_MORE,
    EXCUTE,
    DEL_READ,
    DEL_EXCUTE,
    DEL_WRITE
};

class EventHandler {
   protected:
    typedef struct kevent Event;
    typedef std::vector<Event> EventList;

    // TODO max_event 생성자에서 처리해줄지
    static const int _max_event = 20;
    int _kq_fd;
    int _change_cnt;

    Event _ev_list[_max_event];
    EventList _change_list;

    // HandlerInfo<sockets> _info;
    // Udata<sockets> _udata;

   public:
    EventHandler();
    EventHandler(const EventHandler &copy);
    virtual ~EventHandler();

    int monitorEvent();

    // handle functions
    void handleEvent(int event_idx);
    virtual void handleAccept(int event_idx) = 0;
    virtual void handleRead(int event_idx) = 0;
    virtual void handleExcute(int event_idx) = 0;
    virtual void handleWrite(int event_idx) = 0;

    // void handleAccept(Udata<sockets> &udata);
    // void handleRead(Udata<sockets> &udata);
    // void handleWrite(Udata<sockets> &udata);
    //, int filt, int flags, int action;

    void registerEvent(int fd, int action);
    // void
};
}  // namespace ft

#endif
