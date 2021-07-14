#include <algorithm>
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

size_t getLength(const Int2& size)
{
	return static_cast<size_t>(size.x) * size.y;
}

void Region::setSize(const Int2& size)
{
	size_t oldLength(getLength(Region::size));
	size_t newLength(getLength(size));

	this->size = size;

	if (oldLength == newLength) return;
	if (newLength == 0) ids.reset();
	else ids = std::make_unique<uint8_t[]>(newLength);
}

void Region::copyFrom(const Int2& position, const IdField& field)
{
	for (int32_t y = 0; y < size.y; y++)
	{
		for (int32_t x = 0; x < size.x; x++)
		{
			const Int2 local = Int2(x, y);
			setId(local, field.getId(local + position));
		}
	}
}

void Region::pasteTo(const Int2& position, IdField& field)
{
	for (int32_t y = 0; y < size.y; y++)
	{
		for (int32_t x = 0; x < size.x; x++)
		{
			const Int2 local = Int2(x, y);
			field.setId(local + position, getId(local));
		}
	}
}

void Region::readFrom(std::istream& stream)
{
	Int2 min;
	Int2 max;

	setSize(readBorder(stream, min, max));
	readField(stream, *this, size, Int2(0));
}

void Region::writeTo(std::ostream& stream) const
{
	IdField::writeTo(stream, Int2(0), size - Int2(1));
}

void Region::rotatePositive()
{
	//Rotating a rectangular matrix in place is actually quite difficult
	//So I am going to be lazy and give myself an extra buffer to work with

	auto buffer = std::make_unique<uint8_t[]>(getLength(size));

	for (int32_t y = 0; y < size.y; y++)
	{
		for (int32_t x = 0; x < size.x; x++)
		{
			size_t index = static_cast<size_t>(x) * size.y + y;
			buffer[index] = getId(Int2(size.x - x - 1, y));
		}
	}

	setSize(Int2(size.y, size.x));
	ids = std::move(buffer);
}

void Region::rotateNegative()
{
	//Oh and don't worry about duplicated code lol

	auto buffer = std::make_unique<uint8_t[]>(getLength(size));

	for (int32_t y = 0; y < size.y; y++)
	{
		for (int32_t x = 0; x < size.x; x++)
		{
			size_t index = static_cast<size_t>(x) * size.y + y;
			buffer[index] = getId(Int2(x, size.y - y - 1));
		}
	}

	setSize(Int2(size.y, size.x));
	ids = std::move(buffer);
}

void Region::rotate180()
{
	for (int32_t y = 0; y < (size.y + 1) / 2; y++)
	{
		int32_t width = y * 2 == size.y - 1 ? size.x / 2 : size.x;

		for (int32_t x = 0; x < width; x++)
		{
			Int2 position0(x, size.y - y - 1);
			Int2 position1(size.x - x - 1, y);

			std::swap(ids[getIndex(position0)], ids[getIndex(position1)]);
		}
	}
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
	return (size_t)position.y * size.x + position.x;
}