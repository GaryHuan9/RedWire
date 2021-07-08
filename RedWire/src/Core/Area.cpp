#include "../Type2.h"
#include "Area.h"
#include "Cell.h"
#include "Gate.h"

#include <unordered_map>
#include <functional>
#include <fstream>
#include <cstdint>
#include <memory>
#include <limits>
#include <vector>
#include <iostream>

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
	if (cell == nullptr) return 0xFF020000u;
	return cell->getColor();
}

template<class T> void write(ofstream& stream, const T& value)
{
	stream.write((const char*)&value, sizeof(T));
}

/// <summary>
/// Variable length encode a lane id and lane length pair.
/// </summary>
void write(ofstream& stream, const uint8_t& id, const uint32_t& count)
{
	uint8_t first = id & 0b1111u;
	uint32_t remain = count >> 3;
	first |= (uint8_t)(count << 4);

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

void Area::writeTo(const std::string& path) const
{
	std::ofstream stream(path, std::ifstream::trunc | std::ifstream::binary);
	if (stream.fail()) throw std::exception(("Failed to load " + path).c_str());

	write<uint32_t>(stream, 0); //Write version

	Int2 min;
	Int2 max;

	findBorder(min, max);

	for (int32_t y = min.y; y < max.y; y++)
	{
		uint8_t laneId{ 0u };
		uint32_t count{ 0u };

		for (int32_t x = min.x; x < max.x; x++)
		{
			Cell* const cell = get(Int2(x, y));
			uint8_t id = cell == nullptr ? 0u : cell->getCellId();

			if (laneId == id)
			{
				++count;
				continue;
			}

			if (count > 0) write(stream, laneId, count);

			laneId = id;
			count = 1;
		}

		if (count > 0) write(stream, laneId, count);
	}
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

	max += Int2(1); //Max is exclusive
}