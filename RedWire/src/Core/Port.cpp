#include "Port.h"

using namespace RedWire;


bool Port::getEnabled() const
{
	return enabled;
}

void Port::setEnabled(const bool enabled)
{
	this->enabled = enabled;
}
