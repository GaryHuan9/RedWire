#include <unordered_set>
#include "Type2.h"
#include "Grid.h"
#include "Wire.h"
#include "Port.h"
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
	const auto& pointer = getPtr(position);
	return pointer ? pointer.get() : nullptr;
}

uint32_t Grid::getColor(const Int2& position) const
{
	Cell* const cell = get(position);
	if (cell == nullptr) return 0x000002FFu;
	return cell->getColor();
}

template<class Type> void Grid::removeFrom(vector<shared_ptr<Type>>& vector, Type* target)
{
	auto&& current = vector.begin();
	const auto&& end = vector.end();

	while (current != end && current->get() != target) ++current;
	if (current == end) throw exception("Target item not found!");

	*current = vector.back();
	vector.pop_back();
}

/// Looks through all connected Wire of the same instance and
/// replace them with bundle. NOTE: Returns the positions replaced.
unordered_set<Int2> Grid::floodReplace(const Int2& position, const shared_ptr<Cell>& bundle)
{
	Cell* const target = get(position);
	unordered_set<Int2> replaced;

	Wire* wire = dynamic_cast<Wire*>(target);
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
	Join* const join = dynamic_cast<Join*>(cell);

	if (join != nullptr && join->getEnabled())
	{
		floodSearch(pack, local, direction);
		return;
	}

	if (cell != pack.wire || !pack.replaced.insert(local).second) return;
	for (const Int2& offset : Int2::edges4) floodSearch(pack, local, offset);

	set(local, pack.bundle);
}

void Grid::addWire(const Int2& position)
{
	Cell* previous = get(position);

	if (dynamic_cast<Wire*>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	shared_ptr<Cell> bundle = nullptr;

	for (const Int2& offset : Int2::edges4)
	{
		const Int2 local = position + offset;
		shared_ptr<Cell> neighbor = getPtr(local);

		if (neighbor == nullptr) continue;

		Wire* wire = dynamic_cast<Wire*>(neighbor.get());

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

			static_cast<Wire*>(bundle.get())->combine(*wire);
		}
	}

	if (previous == nullptr)
	{
		shared_ptr<Wire> wire = make_shared<Wire>();

		set(position, wire);
		wires.push_back(wire);
	}

	for (const Int2& offset : Int2::edges4) scanPort(position + offset);
}

void Grid::addGate(const Int2& position)
{
	Cell* previous = get(position);

	if (dynamic_cast<Gate*>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	shared_ptr<Gate> gate = make_shared<Gate>();

	set(position, gate);
	gates.push_back(gate);

	gate->refresh(*this, position);
}

void Grid::addJoin(const Int2& position)
{
	Cell* previous = get(position);

	if (dynamic_cast<Join*>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	shared_ptr<Join> join = make_shared<Join>();

	set(position, join);
	join->refresh(*this, position);
}

void Grid::remove(const Int2& position)
{
	Cell* const previous = get(position);

	//We must use else if because previous might become corruped/undefined when we remove something

	/**/ if (dynamic_cast<Wire*>(previous) != nullptr) removeWire(position);
	else if (dynamic_cast<Gate*>(previous) != nullptr) removeGate(position);
	else if (dynamic_cast<Join*>(previous) != nullptr) removeJoin(position);
}

void Grid::toggleSource(const Int2& position)
{
	Wire* wire = dynamic_cast<Wire*>(get(position));
	if (wire != nullptr) wire->isSource = !wire->isSource;
}

void Grid::removeWire(const Int2& position)
{
	Cell* const previous = get(position);

	set(position, nullptr);

	if (!splitNeighbors(position))
	{
		removeFrom(wires, (Wire*)previous);

		//After the previous method our wire is offically deleted!
		//The variable previous is now invalid and undefined!
	}

	for (const Int2& offset : Int2::edges4) scanPort(position + offset);
}

void Grid::removeGate(const Int2& position)
{
	Cell* const previous = get(position);
	removeFrom(gates, (Gate*)previous);

	set(position, nullptr);
}

void Grid::removeJoin(const Int2& position)
{
	set(position, nullptr);
	splitNeighbors(position);
}

/// <summary>
/// Splits the neighbor connections by making them distinct.
/// </summary>
bool Grid::splitNeighbors(const Int2& position)
{
	unordered_set<Int2> visited;
	unordered_set<Wire*> unique;

	for (const Int2& offset : Int2::edges4)
	{
		Int2 local = position + offset;
		shared_ptr<Cell> neighbor = getPtr(local);

		if (neighbor == nullptr) continue;

		//The the current cell is a Join, we move
		//one block further to check for wires

		Join* join = dynamic_cast<Join*>(neighbor.get());

		if (join != nullptr)
		{
			local = position + offset * 2;
			neighbor = getPtr(local);

			if (neighbor == nullptr) continue;
		}

		//Skip if the wire does not exit or has been visited before

		Wire* wire = dynamic_cast<Wire*>(neighbor.get());
		if (wire == nullptr || visited.count(local) > 0) continue;

		if (!unique.insert(wire).second)
		{
			shared_ptr<Wire> bundle = make_shared<Wire>();
			neighbor = static_pointer_cast<Cell>(bundle);

			bundle->combine(*wire);
			wires.push_back(bundle);
		}

		auto replaced = floodReplace(local, neighbor);
		visited.insert(replaced.begin(), replaced.end());
	}

	return visited.size() > 0;
}

/// <summary>
/// Updates either the gate or join at position
/// by scanning its neighbors/surroundings
/// </summary>
void Grid::scanPort(const Int2& position)
{
	Port* port = dynamic_cast<Port*>(get(position));
	if (port == nullptr) return;

	Gate* const gate = dynamic_cast<Gate*>(port);
	Join* const join = dynamic_cast<Join*>(port);

	if (gate != nullptr) gate->refresh(*this, position);
	if (join != nullptr) join->refresh(*this, position);
}

void Grid::update()
{
	for (shared_ptr<Gate>& gate : gates) gate->update();
	for (shared_ptr<Wire>& wire : wires) wire->update();
}

Int2 getTilePosition(const Int2& position)
{
	static const int size = Grid::Tile::size;

	//Floored integer divide
	Int2&& result = position / size;

	result.x = result.x * size == position.x ? result.x : result.x - (position.x < 0);
	result.y = result.y * size == position.y ? result.y : result.y - (position.y < 0);

	return result;
}

shared_ptr<Cell> Grid::getPtr(const Int2& position) const
{
	Int2&& tilePosition = getTilePosition(position);

	const auto& iterator = tiles.find(tilePosition);
	if (iterator == tiles.end()) return nullptr;

	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	auto& cells = iterator->second.cells;

	return cells[local.x][local.y];
}

void Grid::set(const Int2& position, const shared_ptr<Cell> cell)
{
	Int2&& tilePosition = getTilePosition(position);

	Tile& tile = tiles[tilePosition];
	tilePosition *= Grid::Tile::size;

	const Int2 local = position - tilePosition;
	tile.cells[local.x][local.y] = cell;
}
