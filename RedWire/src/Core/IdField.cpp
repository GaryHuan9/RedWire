#include "IdField.h"
#include "../Type2.h"

#include <istream>
#include <ostream>

using namespace RedWire;
using namespace std;

template<typename T> static void write(ostream& stream, const T& value)
{
	union
	{
		T value;
		char bytes[sizeof(T)];
	} fuse{ value };

	stream.write(fuse.bytes, sizeof(T));
}

/// <summary>
/// Variable length encode a lane id and lane length pair.
/// </summary>
static void write(ostream& stream, const uint8_t& id, const uint32_t& length)
{
	uint8_t first = id & 0b1111u;
	uint32_t remain = length >> 3;
	first |= (uint8_t)(length << 4);

	static const uint32_t Mask = 0b0111'1111u;

	if (remain > 0)
	{
		write(stream, (uint8_t)(first | ~Mask));

		while (remain > Mask)
		{
			write(stream, (uint8_t)(remain | ~Mask));
			remain >>= 7;
		}

		write(stream, (uint8_t)remain);
	}
	else write(stream, (uint8_t)(first & Mask));
}

void IdField::writeTo(ostream& stream, const Int2& min, const Int2& max) const //Max is inclusive
{
	write<uint32_t>(stream, 1); //Write version

	write(stream, min);
	write(stream, max);

	for (int32_t y = min.y; y <= max.y; y++)
	{
		uint8_t laneId{ 0u };
		uint32_t length{ 0u };

		for (int32_t x = min.x; x <= max.x; x++)
		{
			uint8_t id = getId(Int2(x, y));

			if (laneId == id)
			{
				++length;
				continue;
			}

			if (length > 0) write(stream, laneId, length);

			laneId = id;
			length = 1;
		}

		write(stream, laneId, length);
	}
}

template<typename T> static T read(istream& stream)
{
	union
	{
		T value;
		char bytes[sizeof(T)];
	} fuse{};

	stream.read(fuse.bytes, sizeof(T));
	return fuse.value;
}

/// <summary>
/// Variable length decode a lane id and lane length pair.
/// </summary>
static void read(istream& stream, uint8_t& id, uint32_t& length)
{
	uint8_t first = read<uint8_t>(stream);

	id = (uint8_t)(first & 0b1111u);
	length = (uint32_t)(first >> 4);

	static const uint32_t Mask = 0b0111'1111u;

	if (first > Mask)
	{
		length &= 0b0111u;

		for (uint32_t i = 3;; i += 7)
		{
			uint8_t part = read<uint8_t>(stream);
			length |= (uint32_t)(part & Mask) << i;

			if ((part & ~Mask) == 0) break;
		}
	}
}

Int2 IdField::readFrom(istream& stream, IdField& field)
{
	Int2 min;
	Int2 max;

	Int2 size = readBorder(stream, min, max);
	readField(stream, field, size, min);

	return size;
}

Int2 IdField::readFrom(std::istream& stream, IdField& field, const Int2& destination)
{
	Int2 min;
	Int2 max;

	Int2 size = readBorder(stream, min, max);
	readField(stream, field, size, destination);

	return size;
}

Int2 IdField::readBorder(std::istream& stream, Int2& min, Int2& max)
{
	auto version = read<uint32_t>(stream);

	Int2 size;

	if (version == 0)
	{
		size = read<Int2>(stream);

		min = Int2(0);
		max = size - Int2(1);
	}
	else
	{
		min = read<Int2>(stream);
		max = read<Int2>(stream);

		size = max - min + Int2(1);
	}

	return size;
}

void IdField::readField(std::istream& stream, IdField& field, const Int2& size, const Int2& offset)
{
	for (int32_t y = 0; y < size.y; y++)
	{
		uint8_t laneId{ 0u };
		uint32_t length{ 0u };

		for (int32_t x = 0; x < size.x; x++)
		{
			Int2 position = Int2(x, y) + offset;

			if (length == 0) read(stream, laneId, length);
			if (laneId != 0) field.setId(position, laneId);

			--length;
		}
	}
}
