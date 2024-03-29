#include "handler/EventHandler.hpp"

#include <errno.h>

#include <iostream>

#include "core/Type.hpp"
#include "core/Command.hpp"

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
                   NULL);

    _change_list.clear();
    _change_cnt = 0;
    return n;
}

void EventHandler::garbageCollector() {
    handleClose();
}

/**
 * @brief handle event
 *
 * Handle events of detected by monitorEvent
 */
void EventHandler::handleEvent(int event_idx) {
    Event &event = _ev_list[event_idx];
    Client *client = static_cast<Client *>(event.udata);
    e_event action = IDLE;
    if (event.filter == EVFILT_READ)
        client ? action = READ : action = ACCEPT;
    else if (event.filter == EVFILT_WRITE && client)
        action = EXECUTE;
    else if (event.filter == EVFILT_TIMER && client)
        action = TIMER;

    if (event.flags & EV_EOF) {
        _garbage.insert(client);
        std::cout << "# " << event.ident << " EOF" << std::endl;
        return;
    }
    switch (action) {
        case ACCEPT:
            handleAccept();
            break;
        case READ:
            handleRead(event_idx);
            break;
        case EXECUTE:
            handleExecute(event_idx);
            break;
        case TIMER:
            handleTimer(event_idx);
        default:
            std::cout << "client #" << event.ident << "EV_FILT(" << event.filter
                      << "), "
                      << "EV_FLAGS(" << event.flags << ""
                      << ")" << std::endl;
            break;
    }
}

void EventHandler::registerEvent(int fd, short filt, e_event action,
                                 Client *udata) {
    Event ev = {};
    u_short flags = action != D_WRITE && action != D_TIMER ? EV_ADD : EV_DELETE;
    int64_t data = 0;

    if (filt == EVFILT_TIMER && action != D_TIMER) {
        data = 60000;
    }
    EV_SET(&ev, fd, filt, flags, 0, data, static_cast<void *>(udata));
    _change_list.push_back(ev);
    _change_cnt++;
}

}  // namespace ft
