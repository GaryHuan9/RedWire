#include <unordered_set>
#include "Grid.h"
#include "Wire.h"
#include "Type2.h"
#include "Gate.h"
#include "Join.h"

using namespace RedWire;
using namespace std;

Grid::Grid() : tiles(), wires()
{

}

Grid::Tile::Tile() : cells()
{

}

Cell* const Grid::get(const Int2& position) const
{
	shared_ptr<Cell> pointer = getPtr(position);
	return pointer ? pointer.get() : nullptr;
}

uint32_t Grid::getColor(const Int2& position) const
{
	Cell* const cell = get(position);
	if (cell == nullptr) return 0x000003FFu;
	return cell->getColor();
}

template<class Type> void removeFrom(vector<shared_ptr<Type>> vector, Type* target)
{
	auto current = vector.begin();

	while (current != vector.end())
	{
		if (current->get() == target) break;
		++current;
	}

	if (current == vector.end()) throw exception("Not found!");

	*current = vector.back();
	vector.pop_back();
}

/// Looks through all connected Wire of the same instance and
/// replace them with bundle. NOTE: Returns the positions replaced.
unordered_set<Int2> Grid::floodReplace(const Int2& position, const shared_ptr<Cell>& bundle)
{
	Cell* const target = get(position);
	unordered_set<Int2> replaced;

	Wire* const wire = static_cast<Wire* const>(target);
	if (wire == nullptr) return replaced;

	replaced.insert(position);
	set(position, bundle);

	SearchPack pack(wire, replaced, bundle);
	for (const Int2& offset : Int2::edges4) floodSearch(pack, position, offset);

	return replaced;
}

void Grid::floodSearch(const SearchPack& pack, const Int2& source, const Int2& direction)
{
	const Int2 local = source + direction;
	Cell* const cell = get(local);

	if (static_cast<Join* const>(cell) != nullptr) floodSearch(pack, local, direction);
	if (cell != pack.wire || !pack.replaced.insert(local).second) return;

	for (const Int2& offset : Int2::edges4) floodSearch(pack, local, offset);

	set(local, pack.bundle);
}

void Grid::addWire(const Int2& position)
{
	Cell* previous = get(position);

	if (static_cast<Wire*>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	shared_ptr<Cell> bundle = nullptr;

	for (const Int2& offset : Int2::edges4)
	{
		const Int2 local = position + offset;
		shared_ptr<Cell> neighbor = getPtr(local);

		Wire* wire = neighbor == nullptr ? nullptr : static_cast<Wire*>(neighbor.get());

		if (wire == nullptr) continue;

		if (bundle == nullptr)
		{
			bundle = neighbor;
			previous = bundle.get();
			set(position, neighbor);
		}
		else if (wire != bundle.get())
		{
			floodReplace(local, bundle);
			removeFrom(wires, wire);

			static_cast<Wire*>(neighbor.get())->combine(*wire);
		}
	}

	if (previous == nullptr)
	{
		shared_ptr<Wire> wire = make_shared<Wire>();

		set(position, wire);
		wires.push_back(wire);
	}

	for (const Int2& offset : Int2::edges4) scanCrossings(position + offset);
}

void Grid::addGate(const Int2& position)
{}

void Grid::addJoin(const Int2& position)
{}

void Grid::remove(const Int2& position)
{
	Cell* const previous = get(position);

	if (static_cast<Wire*>(previous) != nullptr) removeWire(position);
	if (static_cast<Gate*>(previous) != nullptr) removeGate(position);
	if (static_cast<Join*>(previous) != nullptr) removeJoin(position);
}

void Grid::removeWire(const Int2& position)
{
	Cell* const previous = get(position);

	set(position, nullptr);

	if (splitNeighbors(position))
	{
		removeFrom(wires, (Wire* const)previous);

		//After the previous method our wire is actually deleted!
		//The variable previous is now invalid and undefined!
	}

	scanCrossings(position);
}

void Grid::removeGate(const Int2& position)
{}

void Grid::removeJoin(const Int2& position)
{}

/// <summary>
/// Updates either the gate or join at position
/// by scanning its neighbors/surroundings
/// </summary>
void Grid::scanCrossings(const Int2& position)
{
	Cell* const previous = get(position);
	if (previous == nullptr) return;

	Gate* const gate = static_cast<Gate* const>(previous);
	Join* const join = static_cast<Join* const>(previous);

	if (gate == nullptr && join == nullptr) return;

	//Count all neighbors
	int count = 0;

	for (const Int2& offset : Int2::edges4)
	{
		Cell* const cell = get(position + offset);
		Wire* const wire = static_cast<Wire* const>(cell);

		if (wire != nullptr) ++count;
	}

	if (gate != nullptr)
	{

	}
	else
	{

	}
}

/// <summary>
/// Splits the neighbor connections by making them distinct.
/// </summary>
bool Grid::splitNeighbors(const Int2& position)
{
	unordered_set<Int2> visited;

	for (const Int2& offset : Int2::edges4)
	{
		const Int2 local = position + offset;
		shared_ptr<Cell> neighbor = getPtr(local);

		Wire* wire = neighbor == nullptr ? nullptr : static_cast<Wire*>(neighbor.get());

		if (wire == nullptr || visited.count(local) > 0)
		{
			scanCrossings(local);
			continue;
		}

		if (visited.size() > 0)
		{
			shared_ptr<Wire> bundle = make_shared<Wire>();
			neighbor = static_pointer_cast<Cell>(bundle);

			wires.push_back(bundle);
		}

		auto replaced = floodReplace(local, neighbor);
		visited.insert(replaced.cbegin(), replaced.cend());
	}

	return visited.size() > 0;
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

shared_ptr<Cell> Grid::getPtr(const Int2& position) const
{
	Int2 tilePosition = getTilePosition(position);

	auto iterator = tiles.find(tilePosition);
	if (iterator == tiles.end()) return nullptr;

	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	auto& cells = iterator->second.cells;

	return cells[local.x][local.y];
}

void Grid::set(const Int2& position, const shared_ptr<Cell> cell)
{
	Int2 tilePosition = getTilePosition(position);

	Tile& tile = tiles[tilePosition];
	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	tile.cells[local.x][local.y] = cell;
}
