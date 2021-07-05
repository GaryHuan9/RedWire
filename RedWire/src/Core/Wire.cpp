#include "Wire.h"
#include <random>
#include <limits>

using namespace RedWire;

std::random_device device;
std::mt19937 generator(device());
std::uniform_int_distribution<uint16_t> distribution(255);

Wire::Wire() : isSource(false), poweredLast(false), poweredNext(false)
{
	uint8_t bytes[4]
	{
		255,
		(uint8_t)distribution(generator),
		(uint8_t)distribution(generator),
		(uint8_t)distribution(generator)
	};

	debugColor = *reinterpret_cast<uint32_t*>(bytes);
}

uint32_t Wire::getColor() const
{
	return debugColor;

	if (isSource) return 0xFFAA03FFu;
	if (getPowered()) return 0xFF5503FFu;

	return 0xFF0003FFu;
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