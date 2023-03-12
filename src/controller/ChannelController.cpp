#include "ChannelController.hpp"

namespace ft{
ChannelController::ChannelController()
{
}

ChannelController::ChannelController(const ChannelController &copy)
{
	*this = copy;
}

ChannelController::~ChannelController()
{
}

ChannelController& ChannelController::operator=(const ChannelController &ref)
{
	return (*this);
}

}
