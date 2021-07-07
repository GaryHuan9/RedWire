
#include <stdint.h>
#include <memory>
#include "../Type2.h"
#include "Area.h"
#include "Grid.h"
#include "Cell.h"
#include "Region.h"

using namespace RedWire;

Region::Region(const Int2& size) : size(size), bytes(std::make_unique<uint8_t[]>(static_cast<size_t>(size.x)* size.y))
{

}

void Region::copyFrom(const Int2& position, const Area& area)
{
	for (int x = 0; x < size.x; ++x)
	{
		for (int y = 0; y < size.y; ++y)
		{
			const Int2 local = Int2(x, y);
			Cell* cell = area.get(local + position);

			if (cell == nullptr) set(local, 0);
			else set(local, cell->getCellId());
		}
	}
}

void Region::pasteTo(const Int2& position, Grid& grid)
{
	for (int x = 0; x < size.x; ++x)
	{
		for (int y = 0; y < size.y; ++y)
		{
			const Int2 local = Int2(x, y);
			grid.add(local + position, get(local));
		}
	}
}

size_t Region::getIndex(const Int2& position) const
{
	return static_cast<size_t>(position.x) * size.y + position.y;
}

uint8_t Region::get(const Int2& position) const
{
	return bytes[getIndex(position)];
}

void Region::set(const Int2& position, const uint8_t& byte)
{
	bytes[getIndex(position)] = byte;
}
