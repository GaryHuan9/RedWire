#include "Gate.h"
#include "Grid.h"
#include "Wire.h"

using namespace RedWire;

Gate::Gate() : sourcePosition(), targetPosition(), controlPosition()
{}

uint32_t Gate::getColor() const
{
	return 0xFFFF0C00u;
}

void Gate::update(const std::shared_ptr<Grid> grid) const
{
	Wire* const source = dynamic_cast<Wire* const>(grid->get(sourcePosition));
	Wire* const target = dynamic_cast<Wire* const>(grid->get(targetPosition));
	Wire* const control = dynamic_cast<Wire* const>(grid->get(controlPosition));

	if (source == nullptr || target == nullptr || control == nullptr) return;

	const bool powered = source->getPowered();
	if (control->getPowered() ? !powered : powered) target->setPowered(powered);
}

void Gate::setPositions(const Int2& source, const Int2& target, const Int2& control)
{
	sourcePosition = source;
	targetPosition = target;
	controlPosition = control;
}


