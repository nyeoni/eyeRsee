#include "core/Udata.hpp"

namespace ft {

Udata::Udata() : action(IDLE), status(0), src(NULL) {}
Udata::Udata(const Udata &copy) { *this = copy; }
Udata::~Udata() {}

Udata &Udata::operator=(const Udata &ref) {
    action = ref.action;
    status = ref.status;
    commands = ref.commands;
    src = ref.src;

    return (*this);
}

}  // namespace ft
