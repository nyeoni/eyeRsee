#include "core/Command.hpp"

namespace ft {

Command::Command() : type(UNKNOWN), params(NULL){};
Command::~Command() {
    if (params) {
        delete params;
    }
};

}  // namespace ft
