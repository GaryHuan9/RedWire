#include "Wire.h"
#include <stdint.h>
#include <random>
#include <limits>

using namespace RedWire;

#ifdef _DEBUG

std::random_device device;
std::mt19937 generator(device());
std::uniform_int_distribution<uint16_t> distribution(255);

#endif

Wire::Wire()
{
#ifdef _DEBUG
	uint8_t bytes[4]
	{
		255,
		(uint8_t)distribution(generator),
		(uint8_t)distribution(generator),
		(uint8_t)distribution(generator)
	};

	debugColor = *reinterpret_cast<uint32_t*>(bytes);
#endif
}

uint32_t Wire::getColor() const
{
#ifdef _DEBUG
	//return debugColor;
#endif

	if (isSource) return 0xFF0A55EEu;
	if (getPowered()) return 0xFF0B02FDu;

	return 0xFF020010u;
}

uint8_t Wire::getCellId() const
{
	uint8_t result = 0;

	if (isSource) result |= 1 << 0;
	if (poweredLast) result |= 1 << 1;

	return result + 1; //Should occupy 1-4 (4 variants)
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

void Wire::process(const uint8_t& id)
{
	const uint8_t flag = id - 1;

	isSource = (flag >> 0) & 1;
	poweredLast = (flag >> 1) & 1;
}