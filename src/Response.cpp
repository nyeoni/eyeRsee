#include "Response.hpp"

Response::Response() {}

Response::Response(const Response &copy) { *this = copy; }

Response::~Response() {}

Response &Response::operator=(const Response &ref) { return (*this); }