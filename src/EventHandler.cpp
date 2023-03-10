#include "EventHandler.hpp"

EventHandler::EventHandler() {}

EventHandler::EventHandler(const EventHandler &copy) { *this = copy; }

EventHandler::~EventHandler() {}

EventHandler &EventHandler::operator=(const EventHandler &ref) {
    return (*this);
}

// method

void EventHandler::updateChangeList(int fd) {}
int EventHandler::updateEvent() {}