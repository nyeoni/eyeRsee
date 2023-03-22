#include "core/Udata.hpp"

namespace ft {

Command::Command() : type(UNKNOWN), params(NULL){};
Command::~Command() {
    if (params) {
        delete params;
    }
};

Udata::Udata() : src(NULL) {}
Udata::Udata(const Udata &copy) { *this = copy; }
Udata::~Udata() {}

Udata &Udata::operator=(const Udata &ref) {
    commands = ref.commands;
    src = ref.src;

    return (*this);
}

}  // namespace ft
