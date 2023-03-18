#include <sys/time.h>

#include <iostream>
#include <sstream>
#include <vector>

namespace ft {

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> split(std::istringstream &stream, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

long getTicks(void) {
    struct timeval t_val;

    gettimeofday(&t_val, NULL);
    return (t_val.tv_sec + t_val.tv_usec / 1000000L);
}

}  // namespace ft
