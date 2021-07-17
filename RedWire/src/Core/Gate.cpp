#include "../Type2.h"
#include "Grid.h"
#include "Wire.h"
#include "Port.h"
#include "Gate.h"
#include <assert.h>

using namespace RedWire;

uint32_t Gate::getColor() const
{
	return getEnabled() ? 0xFFEE2200u : 0xFF330400u;
}

uint8_t Gate::getCellId() const
{
	return 5;
}

void Gate::update() const
{
	if (!getEnabled()) return;

	Wire* const source = (Wire*)sourceWire->get();
	Wire* const target = (Wire*)targetWire->get();
	Wire* const control = (Wire*)controlWire->get();

	bool powered = source->getPowered();
	if (control->getPowered() ? !powered : powered) target->setPowered(true);
}