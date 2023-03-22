#include <sys/time.h>

#include <core/utility.hpp>
#include <istream>

namespace ft {

std::vector<std::string> split(std::string str, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;

    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
//        std::cout << token << std::endl;
        str.erase(0, pos + delimiter.length());
    }
//    std::cout << s << std::endl;
    tokens.push_back(str);
    return tokens;
}

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

}  // namespace ft
