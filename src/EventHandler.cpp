#include "EventHandler.hpp"

#include <iostream>

namespace ft {

EventHandler::EventHandler() {}
EventHandler::EventHandler(const EventHandler &copy) { *this = copy; }
EventHandler::~EventHandler() {}

/**
 * @brief monitor events
 */

int EventHandler::monitorEvent() {
    int n = kevent(_kq_fd, &_change_list[0], _change_cnt, _ev_list, _max_event,
                   NULL);
    if (n != -1)
        std::cout << "monitorEvent : " << n << _change_cnt << std::endl;
    _change_list.clear();
    _change_cnt = 0;
    // std::cout << "epoll_wait return " << n << std::endl;
    return n;
}

/**
 * @brief handle events
 */
void EventHandler::handleEvent(int event_idx) {
    if (_ev_list[event_idx].flags == EV_ERROR) {
        std::cerr << "EV_ERROR OCCURED" << std::endl;
        return;
    }
    int action = (int)(intptr_t)_ev_list[event_idx].udata;
    std::cout << "==== handleEvent ====  " << action << std::endl;
    switch (action) {
        case ACCEPT:
            handleAccept(_ev_list[event_idx].ident);

            // after accept -> _socket_list
            std::cout << "ACCEPT detected!" << std::endl;
            break;
        case READ:
            handleRead(_ev_list[event_idx].ident);  // TODO
            // /exit -> _socket_list
            // \n -> parsing -> registerList(EXCUTE)
            std::cout << "READ detected!" << std::endl;
            break;
        case EXCUTE:
            // handleExcute(_ev_list[event_idx].ident);  // TODO

            //  excute -> registerList(WRITE)
            //  PREV_MSG #channel -> channel->client_list
            //  PREV_MSG nickname -> nickname
            //  (X) for (5) send(fd); ->Sync
            break;
        case WRITE:
            // handleWrite(*(UdataWrite *)_ev_list[event_idx].udata);  // TODO

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
            std::cout << "READ" << std::endl;
            break;
        case WRITE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
                   (void *)(intptr_t)action);
            break;
        case DEL_READ:
            EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0,
                   (void *)(intptr_t)action);
            break;
        case DEL_WRITE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0,
                   (void *)(intptr_t)action);
            break;
        case ACCEPT:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   (void *)(intptr_t)action);
            std::cout << "ACCEPT" << std::endl;
            break;
        default:
            return;
            break;
    }
    std::cout << "chlist push_back" << std::endl;
    _change_list.push_back(ev);
    _change_cnt++;
}

}  // namespace ft
