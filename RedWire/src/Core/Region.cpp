
#include <stdint.h>
#include <memory>
#include "Area.h"
#include "Grid.h"
#include "Cell.h"
#include "Region.h"
#include "../Type2.h"

using namespace RedWire;

Region::Region()
{

}

Region::Region(const Int2& size) : Region()
{
	setSize(size);
}

uint32_t Region::getColor(const Int2& position)
{
	uint8_t id = getId(position);
	if (id == 0u) return 0u;

	Int2 offset(id, id);
	uint32_t color = colorsGrid.getColor(offset);

	if (color == 0u)
	{
		colorsGrid.setId(offset, id);
		color = colorsGrid.getColor(offset);
	}

	return color;
}

void Region::setSize(const Int2& size)
{
	size_t oldLength((size_t)this->size.x * this->size.y);
	size_t newLength((size_t)size.x * size.y);

	this->size = size;

	if (oldLength == newLength) return;
	ids = std::make_unique<uint8_t[]>(newLength);
}

void Region::copyFrom(const Int2& position, const IdField& field)
{
	for (int x = 0; x < size.x; ++x)
	{
		for (int y = 0; y < size.y; ++y)
		{
			const Int2 local = Int2(x, y);
			setId(local, field.getId(local + position));
		}
	}
}

void Region::pasteTo(const Int2& position, IdField& field)
{
	for (int x = 0; x < size.x; ++x)
	{
		for (int y = 0; y < size.y; ++y)
		{
			const Int2 local = Int2(x, y);
			field.setId(local + position, getId(local));
		}
	}
}

void Region::writeTo(std::ostream& stream) const
{
	IdField::writeTo(stream, Int2(0), size - Int2(1));
}

void Region::readFrom(std::istream& stream)
{
	Int2 min;
	Int2 max;

	setSize(readBorder(stream, min, max));
	readField(stream, *this, size, Int2(0));
}

uint8_t Region::getId(const Int2& position) const
{
	return ids[getIndex(position)];
}

void Region::setId(const Int2& position, const uint8_t& id)
{
	ids[getIndex(position)] = id;
}

size_t Region::getIndex(const Int2& position) const
{
	return (size_t)position.x * size.y + position.y;
}