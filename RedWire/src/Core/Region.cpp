
#include <stdint.h>
#include <memory>
#include "Type2.h"
#include "Area.h"
#include "Region.h"

using namespace RedWire;

Region::Region(const Int2& size) : bytes(std::make_unique<uint8_t[]>(static_cast<size_t>(size.x)* size.y))
{

}

void Region::copyFrom(const Area& area, const Int2& position)
{
	for (size_t x = 0; x < size.x; ++x)
	{
		for (size_t y = 0; y < size.y; ++y)
		{
			const Int2 local = position + Int2(x, y);

		}
	}
}

size_t Region::getIndex(const Int2& position) const
{
	return static_cast<size_t>(position.y) * size.x + position.x;
}

const uint8_t& Region::get(const Int2& position) const
{
	return bytes[getIndex(position)];
}

void Region::set(const Int2& position, const uint8_t& byte)
{
	bytes[getIndex(position)] = byte;
}
