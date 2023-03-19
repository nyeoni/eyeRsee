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
    e_event action = udata ? udata->action : IDLE;

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
            handleRead(event_idx);  // TODO
            break;
        case EXCUTE:
            handleExecute(event_idx);  // TODO
            break;
        case WRITE:
            handleWrite(event_idx);  // TODO
            break;
        default:
            std::cout << "client #" << _ev_list[event_idx].ident
                      << " (unknown event occured)" << std::endl;
            break;
    }
}

void EventHandler::registerEvent(int fd, e_event action, Udata *udata) {
    Event ev = {};

    // TODO 이것도 모든 경우에 다 맞는지 생각해 주어야 함
    // ACCEPT/CONNECT/READ 따로 EXCUTE/WRITE 따로
    if (udata) udata->action = action;
    switch (action) {
        case ACCEPT:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0,
                   static_cast<void *>(udata));
            break;
        case CONNECT:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0,
                   static_cast<void *>(udata));
            break;
        case READ:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD, 0, 0,
                   static_cast<void *>(udata));
            break;
        case EXCUTE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD, 0, 0,
                   static_cast<void *>(udata));
        case WRITE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0,
                   static_cast<void *>(udata));
            break;
        default:
            return;
            break;
    }
    _change_list.push_back(ev);
    _change_cnt++;
}

}  // namespace ft
