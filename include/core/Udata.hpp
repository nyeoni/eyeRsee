#ifndef UDATA_HPP
#define UDATA_HPP

#include <string>
#include <vector>

#include "Type.hpp"

namespace ft {

class Client;  // TODO 이걸로 모든 컴파일 에러가 해결되는 이유 다같이 생각해보기

struct params {};
struct pass_params : public params {
    std::string password;
};
struct user_params : public params {
    std::string username;
    std::string hostname;
    std::string servername;
    std::string realname;
};
struct nick_params : public params {
    std::string nickname;
    int hopcount;  // optional
};
struct quit_params : public params {
    std::string msg;
};
struct join_params : public params {
    std::vector<std::string> channels;
    std::vector<std::string> keys;
};
struct part_params : public params {
    std::vector<std::string> channels;
};
struct mode_params : public params {
    std::string channel;
    e_mode mode;
};

struct Udata {
    e_event action;
    int status;
    e_cmd command;
    params *params;
    std::string msg;
    Client *src;

    Udata();
    Udata(const Udata &copy);
    ~Udata();
    Udata &operator=(const Udata &ref);
};

// struct UdataBase {
//     int action;
// };
//
// struct UdataRead : public UdataBase {
//     std::string pre_buf;
//     std::vector<std::string> params;
// };
//
// struct UdataWrite : public UdataBase {
//     Response res;
// };

}  // namespace ft

#endif
