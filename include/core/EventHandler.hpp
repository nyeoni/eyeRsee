#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/event.h>

#include <vector>

#include "Udata.hpp"

namespace ft {

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

   public:
    EventHandler();
    EventHandler(const EventHandler &copy);
    virtual ~EventHandler();

    void registerEvent(int fd, e_event action, Udata *udata);

    int monitorEvent();

    // handle functions
    void handleEvent(int event_idx);
    virtual void handleAccept() = 0;
    virtual void handleConnect(int event_idx) = 0;
    virtual void handleRead(int event_idx) = 0;
    virtual void handleExecute(int event_idx) = 0;
    virtual void handleWrite(int event_idx) = 0;
    virtual void handleTimeout(int event_idx) = 0;
};
}  // namespace ft

#endif
