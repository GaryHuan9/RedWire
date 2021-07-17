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

void Gate::refresh(Grid& grid, const Int2& position)
{
	//Count all wire neighbors
	int count = 0;

	for (const Int2& offset : Int2::edges4)
	{
		Cell* const cell = grid.get(position + offset);
		Wire* const wire = dynamic_cast<Wire*>(cell);

		if (wire != nullptr) ++count;
	}

	//Refresh
	setEnabled(count == 3);
	if (!getEnabled()) return;

	Int2 direction;

	for (const Int2& offset : Int2::edges4)
	{
		Cell* const cell = grid.get(position + offset);
		Wire* const wire = dynamic_cast<Wire*>(cell);

		if (wire != nullptr) continue;

		direction = offset * -1;
		break;
	}

	this->grid = &grid;

	Int2 swizzle(-direction.y, direction.x);

	controlWire = grid.getAddress(position + direction);

	sourceWire = grid.getAddress(position + swizzle);
	targetWire = grid.getAddress(position - swizzle);
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

Wire* const Gate::getWire(const Int2& position) const
{
	return static_cast<Wire* const>(grid->get(position));
}
