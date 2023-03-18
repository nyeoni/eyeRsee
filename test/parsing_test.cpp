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

TEST(string_test, string_getline_test) {
    std::string str = "abcd efgh dfdf";
    std::istringstream tokenStream(str);
    std::string token;

    std::getline(tokenStream, token, ' ');
    std::cout << token << std::endl;
    std::getline(tokenStream, token);
    std::cout << token << std::endl;

    std::string input = "   123 456 789   ";
    std::istringstream iss(input);

    std::string trimmed_input;
    std::getline(iss >> std::ws, trimmed_input);
    std::cout << "Input: " << input << std::endl;
    std::cout << "Trimmed input: " << trimmed_input << std::endl;
}

TEST(string_test, istringstream_test) {
    std::string token;

    std::istringstream iss("old string");
    std::getline(iss, token, ' ');

    std::cout << "Before: " << iss.str() << "\n";

    iss.str("new string");
    std::cout << "After: " << iss.str() << "\n";
}

TEST(string_test, commands_parsing_test) {
    std::string buf = "PASS 1234\nNICK nickname\nUSER username hostname servername realname";

    std::vector<std::string> command_lines;
    std::string::size_type sp = 0;
    std::string::size_type ep = buf.find('\n');
    SHOW(ep);
    while (ep != std::string::npos) {
        std::string command_line = buf.substr(sp, ep - sp);
        command_lines.push_back(command_line);
        sp = ep + 1;

        ep = buf.find('\n', sp);
        SHOW(ep);
    }

    for (std::string c : command_lines) {
        std::cout << "cmd: " << c << std::endl;
        std::cout << "===" << std::endl;
    }
}