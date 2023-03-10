#include "Server.hpp"

void Env::parse(int argc, char **argv) {}

Server::Server() {}

Server::Server(const Server &copy) { *this = copy; }

Server::~Server() {}

Server &Server::operator=(const Server &ref) { return (*this); }