#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/event.h>

#include <map>
#include <set>
#include <vector>

#include "core/Command.hpp"

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

    std::set<Client *> _garbage;      // client gone

   public:
    EventHandler();
    EventHandler(const EventHandler &copy);
    virtual ~EventHandler();

    void registerEvent(int fd, short filt, e_event action, Client *udata); // TODO : connect socket

    int monitorEvent();

    void garbageCollector();           // TODO : naming;
    virtual void handleClose() = 0;    // TODO : naming;

    // handle functions
    void handleEvent(int event_idx);
    virtual void handleAccept() = 0;
    virtual void handleRead(int event_idx) = 0;
    virtual void handleExecute(int event_idx) = 0;
    virtual void handleTimer(int event_idx) = 0;
};
}  // namespace ft

#endif
