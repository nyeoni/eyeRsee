#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/event.h>

#include <set>
#include <vector>

#include "core/Udata.hpp"

namespace ft {

class EventHandler {
   protected:
    typedef struct kevent Event;
    typedef std::vector<Event> EventList;

    // TODO max_event 생성자에서 처리해줄지
    static const int _max_event = 20;
    int _kq_fd;
    int _change_cnt;

    std::set<Udata *> _tmp_garbage;  // timeout
    std::set<Udata *> _garbage;      // client gone

    Event _ev_list[_max_event];
    EventList _change_list;

   public:
    EventHandler();
    EventHandler(const EventHandler &copy);
    virtual ~EventHandler();

    void registerEvent(int fd, short filt, e_event action, Udata *udata);

    int monitorEvent();

    void garbageCollector();           // TODO : naming;
    virtual void handleTimeout() = 0;  // TODO : naming;
    virtual void handleClose() = 0;    // TODO : naming;

    // handle functions
    void handleEvent(int event_idx);
    virtual void handleAccept() = 0;
    virtual void handleConnect(int event_idx) = 0;
    virtual void handleRead(int event_idx) = 0;
    virtual void handleExecute(int event_idx) = 0;
    virtual void handleTimer(int event_idx) = 0;
};
}  // namespace ft

#endif
