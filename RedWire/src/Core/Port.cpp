#include "Port.h"

using namespace RedWire;

Port::Port() : enabled(false)
{}

bool Port::getEnabled() const
{
	return enabled;
}

void Port::setEnabled(const bool enabled)
{
	this->enabled = enabled;
}
