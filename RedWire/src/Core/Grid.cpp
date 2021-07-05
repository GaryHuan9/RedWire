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

size_t Grid::getWireCount() const
{
	return wires.size();
}

size_t Grid::getGateCount() const
{
	return gates.size();
}

template<class Type> void removeFrom(vector<shared_ptr<Type>> vector, Type* target)
{
	auto current = vector.begin();

	while (current != vector.end() && current->get() != target) ++current;
	if (current == vector.end()) throw exception("Target item not found!");

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

	scanPort(position);
}

void Grid::addJoin(const Int2& position)
{
	Cell* previous = get(position);

	if (dynamic_cast<Join*>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	set(position, make_shared<Join>());
	scanPort(position);
}

void Grid::remove(const Int2& position)
{
	Cell* const previous = get(position);

	//We must use else if because previous might become corruped/undefined when we remove something

	/**/ if (dynamic_cast<Wire*>(previous) != nullptr) removeWire(position);
	else if (dynamic_cast<Gate*>(previous) != nullptr) removeGate(position);
	else if (dynamic_cast<Join*>(previous) != nullptr) removeJoin(position);
}

void Grid::removeWire(const Int2& position)
{
	Cell* const previous = get(position);

	set(position, nullptr);

	if (splitNeighbors(position))
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

	for (const Int2& offset : Int2::edges4)
	{
		const Int2 local = position + offset;
		shared_ptr<Cell> neighbor = getPtr(local);

		if (neighbor == nullptr) continue;

		Wire* wire = dynamic_cast<Wire*>(neighbor.get());
		if (wire == nullptr || visited.count(local) > 0) continue;

		if (visited.size() > 0)
		{
			shared_ptr<Wire> bundle = make_shared<Wire>();
			neighbor = static_pointer_cast<Cell>(bundle);

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

	//Count all neighbors
	int count = 0;

	for (const Int2& offset : Int2::edges4)
	{
		Cell* const cell = get(position + offset);
		Wire* const wire = dynamic_cast<Wire*>(cell);

		if (wire != nullptr) ++count;
	}

	//Refresh
	Gate* const gate = dynamic_cast<Gate*>(port);
	Join* const join = dynamic_cast<Join*>(port);

	if (gate != nullptr) refreshGate(position, count, *gate);
	if (join != nullptr) refreshJoin(position, count, *join);
}

void Grid::refreshGate(const Int2& position, const int neighborCount, Gate& gate)
{
	gate.setEnabled(neighborCount == 3);
	if (!gate.getEnabled()) return;

	Int2 direction;

	for (const Int2& offset : Int2::edges4)
	{
		Cell* const cell = get(position + offset);
		Wire* const wire = dynamic_cast<Wire*>(cell);

		if (wire != nullptr) continue;

		direction = offset * -1;
		break;
	}

	Int2 swizzle(-direction.y, direction.x);
	Int2 control = position + direction;

	Int2 source = position + swizzle;
	Int2 target = position - swizzle;

	gate.setPositions(source, target, control);
}

void Grid::refreshJoin(const Int2& position, const int neighborCount, Join& join)
{
	join.setEnabled(neighborCount == 4);

	//TODO NOTE: We will change this later so that a T shaped join port will also work

	if (join.getEnabled())
	{
		for (size_t i = 0; i < 2; i++)
		{
			Int2 offset = Int2::edges4[i];
			Int2 local = position + offset;

			Wire* wire = (Wire*)get(local);

			shared_ptr<Cell> bundle = getPtr(position - offset);
			static_cast<Wire*>(bundle.get())->combine(*wire);

			floodReplace(local, bundle);
			removeFrom(wires, wire); //wire is invalid after this point!
		}
	}
	else splitNeighbors(position);
}

void Grid::update()
{
	for (shared_ptr<Gate>& gate : gates) gate->update(*this);
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
