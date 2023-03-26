#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <sstream>
#include <vector>

namespace ft {
std::string getCurrentTime();
std::string toUpperCase(const std::string &str);
std::vector<std::string> split(std::string str, const std::string &delimiter);
std::vector<std::string> split(std::string str, char delimiter);
std::vector<std::string> split(std::istringstream &stream, char delimiter);
}  // namespace ft
#endif  // UTILITY_HPP
