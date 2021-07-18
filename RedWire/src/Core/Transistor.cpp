#include "Transistor.h"
#include "Wire.h"
#include <stdint.h>

using namespace RedWire;

uint32_t Transistor::getColor() const
{
	return getEnabled() ? 0xFF109900u : 0xFF041100u;
}

uint8_t Transistor::getCellId() const
{
	return 8u;
}

void Transistor::update() const
{
	if (!getEnabled()) return;

	Wire* const source = (Wire*)sourceWire->get();
	Wire* const target = (Wire*)targetWire->get();
	Wire* const control = (Wire*)controlWire->get();

	if (source->getPowered() && control->getPowered()) target->setPowered(true);
}
