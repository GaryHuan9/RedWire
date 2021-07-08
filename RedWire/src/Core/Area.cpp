#include "../Type2.h"
#include "Area.h"
#include "Cell.h"

#include <unordered_map>
#include <memory>
#include <fstream>

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

void Area::writeTo(const std::string& path) const
{
	std::ifstream stream(path);

	if (stream.fail()) throw std::exception(("Failed to load " + path).c_str());

	//stream.get()

}

Int2 getTilePosition(const Int2& position)
{
	static const int size = Area::Tile::size;

	//Floored integer divide
	Int2&& result = position / size;

	result.x = result.x * size == position.x ? result.x : result.x - (position.x < 0);
	result.y = result.y * size == position.y ? result.y : result.y - (position.y < 0);

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

	return cells[local.x][local.y];
}

void Area::set(const Int2& position, const shared_ptr<Cell> cell)
{
	Int2&& tilePosition = getTilePosition(position);

	Tile& tile = tiles[tilePosition];
	tilePosition *= Area::Tile::size;

	const Int2 local = position - tilePosition;
	tile.cells[local.x][local.y] = cell;
}
