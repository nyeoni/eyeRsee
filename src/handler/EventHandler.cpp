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
    e_event action = IDLE;
    if (event.filter == EVFILT_READ)
        udata ? action = udata->r_action : action = ACCEPT;
    else if (event.filter == EVFILT_WRITE && udata)
        action = udata->w_action;
    else if (event.filter == EVFILT_TIMER && udata)
        action = udata->timer;

    if (event.flags & EV_EOF) {
        _tmp_garbage.erase(udata);
        _garbage.insert(udata);
        std::cout << "# " << event.ident << " EOF" << std::endl;
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
        case TIMER:
            handleTimer(event_idx);
        default:
            std::cout << "client #" << _ev_list[event_idx].ident
                      << " (unknown event occured)" << std::endl;
            break;
    }
}

void EventHandler::registerEvent(int fd, short filt, e_event action,
                                 Udata *udata) {
    Event ev = {};
    u_short flags = action != D_WRITE && action != D_TIMER ? EV_ADD : EV_DELETE;
    int64_t data = 0;

    if (filt == EVFILT_WRITE) {
        if (action != D_WRITE) udata->w_action = action;
    } else if (filt == EVFILT_READ) {
        if (udata) udata->r_action = action;
    } else {  // EVFILT_TIMER
        if (action != D_TIMER) udata->timer = action;
        data = 5000;
    }
    EV_SET(&ev, fd, filt, flags, 0, data, static_cast<void *>(udata));
    _change_list.push_back(ev);
    _change_cnt++;
}

}  // namespace ft
