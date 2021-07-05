#include "Grid.h"
#include "Wire.h"
#include "Port.h"
#include "Gate.h"
#include <assert.h>

using namespace RedWire;

Gate::Gate() : sourcePosition(), targetPosition(), controlPosition()
{}

uint32_t Gate::getColor() const
{
	return getEnabled() ? 0x0002FFFFu : 0x000233FFu;
}

void Gate::setPositions(const Int2& source, const Int2& target, const Int2& control)
{
	sourcePosition = source;
	targetPosition = target;
	controlPosition = control;
}

void Gate::update(const Grid& grid) const
{
	if (!getEnabled()) return;

	Wire* const source = static_cast<Wire* const>(grid.get(sourcePosition));
	Wire* const target = static_cast<Wire* const>(grid.get(targetPosition));
	Wire* const control = static_cast<Wire* const>(grid.get(controlPosition));

	bool powered = source->getPowered();
	if (control->getPowered() ? !powered : powered) target->setPowered(true);
}


