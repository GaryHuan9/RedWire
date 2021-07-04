#include "Grid.h"

using namespace RedWire;

Grid::Grid() : tiles()
{

}

Int2& getTilePosition(const Int2& position)
{
	const int size = Grid::Tile::size;

	//Floored integer divide
	Int2 result = position / size;

	result.x = result.x * size == position.x ? result.x : result.x - position.x < 0;
	result.y = result.y * size == position.y ? result.y : result.y - position.y < 0;

	return result;
}

const std::shared_ptr<Cell> Grid::operator[](const Int2& position) const
{
	const Int2& tilePosition = getTilePosition(position);

	auto iterator = tiles.find(tilePosition);
	if (iterator == tiles.end()) return nullptr;

	const Int2 local = position - tilePosition * Grid::Tile::size;
	return iterator->second.cells[local.x][local.y];
}

std::shared_ptr<Cell> Grid::operator[](const Int2& position)
{
	const Int2& tilePosition = getTilePosition(position);

	auto iterator = tiles.find(tilePosition);
	if (iterator == tiles.end()) return nullptr;

	const Int2 local = position - tilePosition * Grid::Tile::size;
	return iterator->second.cells[local.x][local.y];
}
