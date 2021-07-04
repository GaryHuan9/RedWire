#include "Grid.h"

using namespace RedWire;

Grid::Grid() : tiles()
{

}

Int2& getTilePosition(const Int2& position)
{
	const int size = Grid::Tile::size;

	//Floored integer divide
	Int2 result = position / Int2{ size, size };

	result.x = result.x * size == position.x ? result.x : result.x - position.x < 0;
	result.y = result.y * size == position.y ? result.y : result.y - position.y < 0;

	return result;
}

const Cell& Grid::operator[](const Int2& position) const
{
	auto iterator = tiles.find(getTilePosition(position));
	if (iterator == tiles.end()) return Cell::null;

	const Tile tile = iterator->second;
	return Cell::null;
}
