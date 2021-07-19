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

	Wire* const source = (Wire*)wire1->get();
	Wire* const target = (Wire*)wire0->get();
	Wire* const control = (Wire*)controlWire->get();

	if (source->getPowered() && control->getPowered()) target->setPowered(true);
}
