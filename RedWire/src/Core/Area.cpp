#include "../Type2.h"
#include "Area.h"
#include "Cell.h"
#include "Grid.h"

#include <unordered_map>
#include <functional>
#include <fstream>
#include <cstdint>
#include <memory>
#include <limits>
#include <vector>

using namespace RedWire;
using namespace std;

Area::Area() : tiles()
{

}

Area::Tile::Tile() : cells()
{

}

Cell* const Area::get(const Int2& position) const
{
	const auto& pointer = getPtr(position);
	return pointer ? pointer.get() : nullptr;
}

uint32_t Area::getColor(const Int2& position) const
{
	Cell* const cell = get(position);
	if (cell == nullptr) return 0u;
	return cell->getColor();
}

bool searchX(const vector<reference_wrapper<const Area::Tile>>& tiles, const size_t& y)
{
	for (const Area::Tile& tile : tiles)
	{
		for (size_t x = 0; x < Area::Tile::size; x++)
		{
			if (tile.cells[y][x]) return true;
		}
	}

	return false;
}

bool searchY(const vector<reference_wrapper<const Area::Tile>>& tiles, const size_t& x)
{
	for (const Area::Tile& tile : tiles)
	{
		for (size_t y = 0; y < Area::Tile::size; y++)
		{
			if (tile.cells[y][x]) return true;
		}
	}

	return false;
}

void Area::findBorder(Int2& min, Int2& max) const
{
	static const int32_t TileSize = Tile::size;

	//Find border tiles first
	Int2 minTile(numeric_limits<int32_t>::max());
	Int2 maxTile(numeric_limits<int32_t>::min());

	vector<reference_wrapper<const Tile>> minX;
	vector<reference_wrapper<const Tile>> minY;
	vector<reference_wrapper<const Tile>> maxX;
	vector<reference_wrapper<const Tile>> maxY;

	for (auto& pair : tiles)
	{
		const Int2& tile = pair.first;

		if (tile.x < minTile.x) { minX.clear(); minTile.x = tile.x; }
		if (tile.y < minTile.y) { minY.clear(); minTile.y = tile.y; }
		if (tile.x > maxTile.x) { maxX.clear(); maxTile.x = tile.x; }
		if (tile.y > maxTile.y) { maxY.clear(); maxTile.y = tile.y; }

		if (tile.x == minTile.x) minX.push_back(pair.second);
		if (tile.y == minTile.y) minY.push_back(pair.second);
		if (tile.x == maxTile.x) maxX.push_back(pair.second);
		if (tile.y == maxTile.y) maxY.push_back(pair.second);
	}

	//Search border tiles for border cells
	for (int32_t x = 0; x < TileSize; x++)
	{
		if (!searchY(minX, x)) continue;
		min.x = x + minTile.x * TileSize;

		break;
	}

	for (int32_t y = 0; y < TileSize; y++)
	{
		if (!searchX(minY, y)) continue;
		min.y = y + minTile.y * TileSize;

		break;
	}

	for (int32_t x = TileSize - 1; x >= 0; x--)
	{
		if (!searchY(maxX, x)) continue;
		max.x = x + maxTile.x * TileSize;

		break;
	}

	for (int32_t y = TileSize - 1; y >= 0; y--)
	{
		if (!searchX(maxY, y)) continue;
		max.y = y + maxTile.y * TileSize;

		break;
	}
}

template<typename T> void write(ostream& stream, const T& value)
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
void write(ostream& stream, const uint8_t& id, const uint32_t& length)
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

void Area::writeTo(ostream& stream, const Int2& min, const Int2& max) const //Max is inclusive
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
			Cell* const cell = get(Int2(x, y));
			uint8_t id = cell == nullptr ? 0u : cell->getCellId();

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

void Area::writeTo(ostream& stream, const Float2& viewCenter, const float& viewExtend) const
{
	Int2 min;
	Int2 max;

	findBorder(min, max);
	writeTo(stream, min, max);

	write<uint8_t>(stream, 1); //Write one to indicate that view data is stored

	write(stream, viewCenter);
	write(stream, viewExtend);
}

template<typename T> T read(istream& stream)
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
void read(istream& stream, uint8_t& id, uint32_t& length)
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

Int2 read(std::istream& stream, Grid& grid, const Int2 const* destination)
{
	auto version = read<uint32_t>(stream);

	Int2 min;
	Int2 size;

	if (version == 0)
	{
		min = Int2(0);
		size = read<Int2>(stream);
	}
	else
	{
		min = read<Int2>(stream);
		size = read<Int2>(stream) - min + Int2(1);
	}

	Int2 offset = destination == nullptr ? min : *destination;

	for (int32_t y = 0; y < size.y; y++)
	{
		uint8_t laneId{ 0u };
		uint32_t length{ 0u };

		for (int32_t x = 0; x < size.x; x++)
		{
			Int2 position = Int2(x, y) + offset;

			if (length == 0) read(stream, laneId, length);
			if (laneId != 0) grid.add(position, laneId);

			--length;
		}
	}

	return size;
}

Int2 Area::readFrom(istream& stream, Grid& grid)
{
	return read(stream, grid, nullptr);
}

Int2 Area::readFrom(std::istream& stream, Grid& grid, const Int2& destination)
{
	return read(stream, grid, &destination);
}

unique_ptr<Grid> Area::readFrom(istream& stream)
{
	auto grid = make_unique<Grid>();
	readFrom(stream, *grid);
	return grid;
}

unique_ptr<Grid> Area::readFrom(istream& stream, Float2& viewCenter, float& viewExtend)
{
	auto grid = readFrom(stream);

	if (read<uint8_t>(stream) > 0)
	{
		viewCenter = read<Float2>(stream);
		viewExtend = read<float>(stream);
	}

	return grid;
}

Int2 Area::getTilePosition(const Int2& position)
{
	//Floored integer divide
	static const int Size = Tile::size;
	Int2&& result = position / Size;

	result.x = result.x * Size == position.x ? result.x : result.x - (position.x < 0);
	result.y = result.y * Size == position.y ? result.y : result.y - (position.y < 0);

	return result;
}

shared_ptr<Cell> Area::getPtr(const Int2& position) const
{
	Int2&& tilePosition = getTilePosition(position);

	const auto& iterator = tiles.find(tilePosition);
	if (iterator == tiles.end()) return nullptr;

	tilePosition *= Area::Tile::size;

	const Int2 local = position - tilePosition;
	auto& cells = iterator->second.cells;

	return cells[local.y][local.x];
}

void Area::set(const Int2& position, const shared_ptr<Cell> cell)
{
	Int2&& tilePosition = getTilePosition(position);

	Tile& tile = tiles[tilePosition];
	tilePosition *= Area::Tile::size;

	const Int2 local = position - tilePosition;
	auto& target = tile.cells[local.y][local.x];

	bool hasTarget = (bool)target;
	bool hasAssign = (bool)cell;

	if (!hasTarget && hasAssign) ++tile.cellCount;
	if (hasTarget && !hasAssign) --tile.cellCount;

	if (tile.cellCount > 0) target = cell;
	else tiles.erase(tilePosition / Area::Tile::size);
}