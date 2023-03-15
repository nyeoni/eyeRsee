#include "gtest/gtest.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#define SHOW(...) \
    std::cout << std::setw(29) << #__VA_ARGS__ << " == " << __VA_ARGS__ << '\n'

TEST(buf_test, buf_persistence_test) {
    std::cout << "buf_persistence_test" << std::endl;
    std::ifstream
        is("/Users/chloek/42Seoul/ft_irc/test/test.txt", std::ifstream::binary);

    std::cout << "good: " << is.good() << std::endl;
    if (is) {
        // get length of file:
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        char *buf = new char[4]; // 이것부터 의문

        std::cout << "Reading " << length << " chars....." << std::endl;

        is.read(buf, 2);
        std::cout << buf << std::endl;

        is.read(buf, 1);
        std::cout << buf << std::endl;
    }
}

TEST(string_test, string_find_test) {
    std::string str = "fuckyouirc";

    std::string::size_type found1 = str.find('\n');
    SHOW(found1);
    if (found1 == std::string::npos)
        std::cout << "nono" << std::endl;

    std::string::size_type found2 = str.find('i');
    SHOW(found2);
}

TEST(string_test, string_substr_test) {
    std::string str = "We think in generalities, but we live in details.";

    std::string::size_type pos = str.find('t');

    std::string str2 = str.substr(0, pos);
    std::string tmp = str.substr(2, str.length());
    SHOW(tmp);

    SHOW(str);
    SHOW(str2);
}