#include <iostream>
#include <sstream>
#include <vector>

namespace ft {
std::vector<std::string> split(std::string str, char Delimiter) {
    std::istringstream iss(str);
    std::string buffer;

    std::vector<std::string> result;

    while (std::getline(iss, buffer, Delimiter)) {
        result.push_back(buffer);
    }

    return result;
}
}  // namespace ft
