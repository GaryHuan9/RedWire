#include "Grid.h"

using namespace RedWire;

Grid::Grid() : tiles(), wires()
{

}

Grid::Tile::Tile() : cells()
{

}

Int2 getTilePosition(const Int2& position)
{
	const int size = Grid::Tile::size;

	//Floored integer divide
	Int2 result = position / size;

	result.x = result.x * size == position.x ? result.x : result.x - (position.x < 0);
	result.y = result.y * size == position.y ? result.y : result.y - (position.y < 0);

	return result;
}

Cell* const Grid::get(const Int2& position) const
{
	Int2 tilePosition = getTilePosition(position);

	auto iterator = tiles.find(tilePosition);
	if (iterator == tiles.end()) return nullptr;

	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	return iterator->second.cells[local.x][local.y].get();
}

uint32_t Grid::getColor(const Int2& position) const
{
	const Cell* cell = get(position);
	if (cell == nullptr) return 0xFF0C0000u;
	return cell->getColor();
}

void Grid::addWire(const Int2& position)
{

}

void Grid::addGate(const Int2& position)
{}

void Grid::remove(const Int2& position)
{}

void Grid::set(const Int2& position, const std::shared_ptr<Cell> cell)
{
	Int2 tilePosition = getTilePosition(position);

	Tile& tile = tiles[tilePosition];
	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	tile.cells[local.x][local.y] = cell;
}
