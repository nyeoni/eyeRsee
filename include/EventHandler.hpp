#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

class EventHandler {
   private:
    int kq_fd;
    kevent *change_list[];  // TODO std::vector<kevent> ?
    int change_cnt;
    const int max_event;

   public:
    EventHandler(/* args*/);
    EventHandler(const EventHandler &copy);
    ~EventHandler();
    EventHandler &operator=(const EventHandler &ref);

    // methods
    void updateChangeList(int fd);
    int updateEvent();
};

#endif
