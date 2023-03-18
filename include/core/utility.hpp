#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <sstream>
#include <vector>

namespace ft {

std::vector<std::string> split(std::string str, char delimiter);
std::vector<std::string> split(std::istringstream& stream, char delimiter);
long long getTicks(void);
}  // namespace ft
#endif  // UTILITY_HPP
