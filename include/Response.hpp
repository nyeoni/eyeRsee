#ifndef RESPONSE_HPP
#define RESPONSE_HPP

struct Response {
    char *buf;
    size_t len;
    size_t current;
};

#endif
