#include "core/EventHandler.hpp"
#include "core/Udata.hpp"
#include "core/Type.hpp"
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
 * @brief handle event
 *
 * Handle events of detected by monitorEvent
 */
void EventHandler::handleEvent(int event_idx) {
    Event event = _ev_list[event_idx];
    int action = ((Udata *) event.udata)->action;

    if (event.flags == EV_ERROR) {
        std::cerr << "EV_ERROR OCCURED" << std::endl;
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

    // TODO udata 어디서 new 해줄지 생각하기
    if (!udata) udata = new Udata();
    // TODO 이것도 모든 경우에 다 맞는지 생각해 주어야 함
    udata->action = action;
    switch (action) {
        case ACCEPT:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   (void *) udata);
            break;
        case CONNECT:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   (void *) udata);
            break;
        case READ:
            EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0,
                   (void *) udata);
            break;
        case EXCUTE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
                   (void *) udata);
        case WRITE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0,
                   (void *) udata);
            break;
        case DEL_READ:
            EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0,
                   (void *) (intptr_t) READ);
            break;
        case DEL_WRITE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0,
                   (void *) (intptr_t) WRITE);
            break;
        case DEL_EXCUTE:
            EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0,
                   (void *) (intptr_t) EXCUTE);
            break;
        default:
            return;
            break;
    }
    _change_list.push_back(ev);
    _change_cnt++;
}

}  // namespace ft
