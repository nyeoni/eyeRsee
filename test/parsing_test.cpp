#include "gtest/gtest.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

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

        char *buf = new char[2]; // 이것부터 의문

        std::cout << "Reading " << length << " chars....." << std::endl;

        is.read(buf, 2);
        std::cout << buf << std::endl;

        is.read(buf, 2);
        std::cout << buf << std::endl;
    }
}