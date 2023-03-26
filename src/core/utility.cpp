
#include <core/utility.hpp>
#include <cctype>
#include <iostream>

namespace ft {

std::string getCurrentTime() {
    long time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time_str = ctime(&time);
    
    return time_str;
}

std::string toUpperCase(const std::string &str) {
    std::string result;
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        result += toupper(*it);
    }
    return result;
}

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
