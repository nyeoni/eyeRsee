#include "controller/BotController.hpp"

namespace ft {

BotController::BotController() {}
BotController::BotController(const BotController &copy) { *this = copy; }
BotController::~BotController() {}
BotController &BotController::operator=(const BotController &ref) { return *this; }

std::string BotController::help() {
    std::stringstream msg_stream;

    msg_stream << "<< (๑╹ワ╹) eyeRsee bot help >> "
               << "* help" << " : " << "show bot commands. "
               << "* hi" << " : " << "say hi to eyeRsee bot. "
               << "* now" << " : " << "show bot commands";
    return msg_stream.str();
}

std::string BotController::hi() {
    std::stringstream msg_stream;

    msg_stream << "(｡♥‿♥｡) Hi~~ Welcome to eyeRsee world ~ ( ´͈ ᵕ `͈ )◞♡";
    return msg_stream.str();
}

std::string BotController::bye() {
    std::stringstream msg_stream;

    msg_stream << "( ☉_☉) Don't Go ~ ＿ø(●ʘ╻ʘ●)";
    return msg_stream.str();
}

std::string BotController::now() {
    std::stringstream msg_stream;

    msg_stream << "ฅ(๑•̀⩊•́๑)ฅ Today is " << getCurrentTime();
    return msg_stream.str();
}

std::string BotController::weather() {
    std::stringstream msg_stream;

    msg_stream << "₍ᐢ• ‧̫ •̥ᐢ₎ Check here >>> " << "https://www.weather.go.kr/w/index.do";
    return msg_stream.str();
}

} // namespace ft


