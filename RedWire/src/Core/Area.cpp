#include "Area.h"
#include "Cell.h"
#include "Grid.h"
#include "IdField.h"

#include <cstdint>
#include <istream>
#include <ostream>
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

const shared_ptr<Cell>* Area::getAddress(const Int2& position) const
{
	Int2&& tilePosition = getTilePosition(position);
	const auto& iterator = tiles.find(tilePosition);

	if (iterator == tiles.end()) return nullptr;
	tilePosition *= Area::Tile::size;

	const Int2 local = position - tilePosition;
	auto& cells = iterator->second.cells;

	return &cells[local.y][local.x];
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

void Area::writeTo(ostream& stream, const Float2& viewCenter, const float& viewExtend) const
{
	Int2 min;
	Int2 max;

	findBorder(min, max);
	IdField::writeTo(stream, min, max);

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

unique_ptr<Grid> Area::readFrom(istream& stream)
{
	auto grid = make_unique<Grid>();
	IdField::readFrom(stream, *grid);
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
	auto address = getAddress(position);
	return address ? *address : nullptr;
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