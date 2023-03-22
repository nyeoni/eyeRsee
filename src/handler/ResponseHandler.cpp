#include "handler/ResponseHandler.hpp"

namespace ft {

const std::string ResponseHandler::servername = "localhost";

const std::string ResponseHandler::RPL_WELCOME_MSG = "Welcome to the Internet Relay Network";
const std::string ResponseHandler::RPL_YOURHOST_MSG = "Your host is eyeRsee.local, running version 1.0";
const std::string ResponseHandler::RPL_CREATED_MSG = "This server was created 15:58:02 Mar 20 2023";
const std::string ResponseHandler::RPL_MYINFO_MSG = "eyeRsee.local 1.0 iosw biklmnopstv";

const std::string ResponseHandler::RPL_NOTOPIC_MSG = "No topic is set";
const std::string ResponseHandler::RPL_TOPIC_MSG = "";

}