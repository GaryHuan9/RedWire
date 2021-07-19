#include "../Type2.h"
#include "Grid.h"
#include "Wire.h"
#include "Port.h"
#include "Inverter.h"
#include <stdint.h>

using namespace RedWire;

uint32_t Inverter::getColor() const
{
	return getEnabled() ? 0xFFEE2200u : 0xFF330400u;
}

uint8_t Inverter::getCellId() const
{
	return 5u;
}

void Inverter::update() const
{
	if (!getEnabled()) return;

	Wire* const source = (Wire*)wire0->get();
	Wire* const target = (Wire*)wire1->get();
	Wire* const control = (Wire*)controlWire->get();

	bool powered = source->getPowered();
	if (control->getPowered() ? !powered : powered) target->setPowered(true);
}