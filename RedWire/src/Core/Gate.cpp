#include "Gate.h"
#include "Grid.h"
#include "Wire.h"
#include <assert.h>

using namespace RedWire;

Gate::Gate() : enabled(), sourcePosition(), targetPosition(), controlPosition()
{}

uint32_t Gate::getColor() const
{
	return 0x000CFFFFu;
}

void Gate::disable()
{
	enabled = false;
}

void Gate::setPositions(const Int2& source, const Int2& target, const Int2& control)
{
	sourcePosition = source;
	targetPosition = target;
	controlPosition = control;
}

void Gate::update(const std::shared_ptr<Grid> grid) const
{
	if (!enabled) return;

	Wire* const source = static_cast<Wire* const>(grid->get(sourcePosition));
	Wire* const target = static_cast<Wire* const>(grid->get(targetPosition));
	Wire* const control = static_cast<Wire* const>(grid->get(controlPosition));

	assert(source != nullptr && target != nullptr && control != nullptr);

	const bool powered = source->getPowered();
	if (control->getPowered() ? !powered : powered) target->setPowered(powered);
}


