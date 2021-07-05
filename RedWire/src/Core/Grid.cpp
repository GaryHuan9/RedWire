#include <unordered_set>
#include "Grid.h"
#include "Wire.h"
#include "Int2.h"

using namespace RedWire;

Grid::Grid() : tiles(), wires()
{

}

Grid::Tile::Tile() : cells()
{

}

Cell* const Grid::get(const Int2& position) const
{
	const std::shared_ptr<Cell>* const ptr = getPtr(position);
	return ptr == nullptr ? nullptr : ptr->get();
}

uint32_t Grid::getColor(const Int2& position) const
{
	Cell* const cell = get(position);
	if (cell == nullptr) return 0x00000CFFu;
	return cell->getColor();
}

template<class Type> void removeFrom(std::vector<std::shared_ptr<Type>> vector, Type* target)
{
	auto current = vector.begin();

	while (current != vector.end())
	{
		if (current->get() == target) break;
		++current;
	}

	if (current == vector.end()) throw std::exception("Not found!");

	*current = vector.back();
	vector.pop_back();
}

/// Looks through all connected Wire of the same instance and
/// replace them with bundle. NOTE: Returns the positions replaced.
const std::unordered_set<Int2> Grid::floodReplace(const Int2& position, const Wire* bundle)
{
	std::unordered_set<Int2> replaced;

	//Cell* target = get(position);

	return replaced;
}

void Grid::addWire(const Int2& position)
{
	Cell* const previous = get(position);

	if (static_cast<Wire* const>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	Wire* bundle = nullptr;

	for (const Int2& offset : Int2::edges4)
	{
		const Int2 local = position + offset;

		auto& const neighborPtr = *getPtr(local);
		Cell* const neighbor = neighborPtr.get();

		Wire* const wire = static_cast<Wire* const>(neighbor);
		if (wire == nullptr) continue;

		if (bundle == nullptr)
		{
			bundle = wire;
			set(position, neighborPtr);
		}
		else if (wire != bundle)
		{
			floodReplace(local, bundle);
			removeFrom(wires, wire);
			bundle->combine(*wire);
		}
	}

	if (previous == nullptr)
	{
		std::shared_ptr<Wire> wire = std::make_shared<Wire>();

		set(position, wire);
		wires.push_back(wire);
	}
}

void Grid::addGate(const Int2& position)
{}

void Grid::addJunction(const Int2& position)
{}

void Grid::remove(const Int2& position)
{}

void Grid::removeWire(const Int2& position)
{}

void Grid::removeGate(const Int2& position)
{}

void Grid::removeJunction(const Int2& position)
{}

Int2 getTilePosition(const Int2& position)
{
	const int size = Grid::Tile::size;

	//Floored integer divide
	Int2 result = position / size;

	result.x = result.x * size == position.x ? result.x : result.x - (position.x < 0);
	result.y = result.y * size == position.y ? result.y : result.y - (position.y < 0);

	return result;
}

const std::shared_ptr<Cell>* const Grid::getPtr(const Int2& position) const
{
	Int2 tilePosition = getTilePosition(position);

	auto iterator = tiles.find(tilePosition);
	if (iterator == tiles.end()) return nullptr;

	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	return &iterator->second.cells[local.x][local.y];
}

void Grid::set(const Int2& position, const std::shared_ptr<Cell> cell)
{
	Int2 tilePosition = getTilePosition(position);

	Tile& tile = tiles[tilePosition];
	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	tile.cells[local.x][local.y] = cell;
}
