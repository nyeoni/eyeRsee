#include "core/Udata.hpp"

namespace ft {

Udata::Udata() : action(IDLE), status(0), params(NULL), src(NULL) {}
Udata::Udata(const Udata &copy) { *this = copy; }
Udata::~Udata() {}
Udata &Udata::operator=(const Udata &ref) {
    action = ref.action;
    status = ref.status;
    command = ref.command;
    params = ref.params;
    msg = ref.msg;
    src = ref.src;
}

}