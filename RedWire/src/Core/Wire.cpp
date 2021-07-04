#include "Wire.h"

using namespace RedWire;

Wire::Wire() : isSource(false), poweredLast(false), poweredNext(false)
{

}

uint32_t Wire::getColor() const
{
	if (isSource) return 0xFF0CAAFFu;
	if (getPowered()) return 0xFF0C55FFu;

	return 0xFF0C00FFu;
}

bool Wire::getPowered() const
{
	return poweredLast || isSource;
}

void Wire::setPowered(bool powered)
{
	poweredNext = powered;
}

void Wire::update()
{
	poweredLast = poweredNext;
	poweredNext = false;
}

void Wire::combine(const Wire& wire)
{
	isSource |= wire.isSource;
	poweredLast |= wire.poweredLast;
	poweredNext |= wire.poweredNext;
}