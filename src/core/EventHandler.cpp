#include "core/EventHandler.hpp"
#include <iostream>

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

/**
 * @brief handle events
 *
 * Handle events of detected by monitorEvent
 */
void EventHandler::handleEvent(int event_idx) {
    if (_ev_list[event_idx].flags == EV_ERROR) {
        std::cerr << "EV_ERROR OCCURED" << std::endl;
        return;
    }
    int action = (int)(intptr_t)_ev_list[event_idx].udata;
    switch (action) {
        case ACCEPT:
            handleAccept();
            // after accept -> _socket_list
            break;
        case READ:
            handleRead(event_idx);  // TODO
            // /exit -> _socket_list
            // \n -> parsing -> registerList(EXCUTE)
            break;
        case EXCUTE:
            handleExcute(event_idx);  // TODO

            //  excute -> registerList(WRITE)
            //  PREV_MSG #channel -> channel->client_list
            //  PREV_MSG nickname -> nickname
            //  (X) for (5) send(fd); ->Sync
            break;
        case WRITE:
            handleWrite(event_idx);  // TODO

            // send(fd); -> ASync
            break;
        // case WRITE:
        //     break;
        default:
            std::cout << "client #" << _ev_list[event_idx].ident
                      << " (unknown event occured)" << std::endl;
    }
}

void EventHandler::registerEvent(int fd, int action) {
    struct kevent ev;

    switch (action) {
        case READ:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   (void *)(intptr_t)action);
            break;
        case EXCUTE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
                   (void *)(intptr_t)action);
        case WRITE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
                   (void *)(intptr_t)action);
            break;
        case DEL_READ:
            EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0,
                   (void *)(intptr_t)READ);
            break;
        case DEL_WRITE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0,
                   (void *)(intptr_t)WRITE);
            break;
        case DEL_EXCUTE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0,
                   (void *)(intptr_t)EXCUTE);
            break;
        case ACCEPT:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   (void *)(intptr_t)action);
            break;
        default:
            return;
            break;
    }
    _change_list.push_back(ev);
    _change_cnt++;
}

}  // namespace ft