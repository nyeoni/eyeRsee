#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

struct Response {
    std::string *buf;
    size_t len;
    size_t current;
};

#endif
