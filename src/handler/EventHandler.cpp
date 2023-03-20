#include "handler/EventHandler.hpp"

#include <errno.h>

#include <iostream>

#include "core/Type.hpp"
#include "core/Udata.hpp"

namespace ft {
EventHandler::EventHandler() : _change_cnt(0) {
    _kq_fd = kqueue();
    _change_list.reserve(128);
}
EventHandler::EventHandler(const EventHandler &copy) { *this = copy; }
EventHandler::~EventHandler() {}

/**
 * @brief monitor events
 * @return number of monitored events
 */

int EventHandler::monitorEvent() {
    int n = kevent(_kq_fd, &_change_list[0], _change_cnt, _ev_list, _max_event,
                   NULL);  // TODO : TIMEOUT

    _change_list.clear();
    _change_cnt = 0;
    return n;
}

void EventHandler::garbageCollector() {
    handleTimeout();
    handleClose();
}

/**
 * @brief handle event
 *
 * Handle events of detected by monitorEvent
 */
void EventHandler::handleEvent(int event_idx) {
    Event event = _ev_list[event_idx];
    Udata *udata = static_cast<Udata *>(event.udata);
    e_event action = udata ? udata->r_action : ACCEPT;
    if (event.filter == EVFILT_WRITE) action = udata->w_action;

    if ((event.flags & EV_EOF) && udata && !isConnected(udata)) {
        // TODO : ctrl-D 처리
        _unregisters.erase(udata);
        _garbage.insert(udata);
        std::cout << "# " << event.ident << " EOF with no recv buf"
                  << std::endl;
        return;
    }
    switch (action) {
        case ACCEPT:
            handleAccept();
            break;
        case CONNECT:
            handleConnect(event_idx);
            break;
        case READ:
            handleRead(event_idx);
            break;
        case EXECUTE:
            handleExecute(event_idx);
            break;
        case WRITE:
            handleWrite(event_idx);
            break;
        default:
            std::cout << "client #" << _ev_list[event_idx].ident
                      << " (unknown event occured)" << std::endl;
            break;
    }
}

void EventHandler::registerEvent(int fd, e_filt filt, e_event action,
                                 Udata *udata) {
    Event ev = {};

    if (filt == FILT_WRITE) {
        if (action == D_WRITE)
            EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
        else {
            udata->w_action = action;
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD, 0, 0,
                   static_cast<void *>(udata));
        }
    } else {
        if (udata) {
            udata->r_action = action;
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   static_cast<void *>(udata));
        } else
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    }
    _change_list.push_back(ev);
    _change_cnt++;
}

}  // namespace ft
