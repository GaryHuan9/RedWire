#include "Grid.h"
#include "Wire.h"
#include "Port.h"
#include "Gate.h"
#include "Bridge.h"
#include "Note.h"

using namespace RedWire;
using namespace std;

Grid::Grid() : wires(), gates()
{

}

template<typename Type> void Grid::removeFrom(vector<shared_ptr<Type>>& vector, Type* target)
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
	Bridge* const bridge = dynamic_cast<Bridge*>(cell);

	if (bridge != nullptr && bridge->getEnabled())
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

void Grid::addBridge(const Int2& position)
{
	Cell* previous = get(position);

	if (dynamic_cast<Bridge*>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	shared_ptr<Bridge> bridge = make_shared<Bridge>();

	set(position, bridge);
	bridge->refresh(*this, position);
}

void Grid::addNote(const Int2& position)
{
	Cell* previous = get(position);

	if (dynamic_cast<Note*>(previous) != nullptr) return;
	if (previous != nullptr) remove(position);

	shared_ptr<Note> note = make_shared<Note>();

	set(position, note);
}

void Grid::remove(const Int2& position)
{
	Cell* const previous = get(position);

	//We must use else if because previous might become corruped/undefined when we remove something

	/**/ if (dynamic_cast<Wire*>(previous) != nullptr) removeWire(position);
	else if (dynamic_cast<Gate*>(previous) != nullptr) removeGate(position);
	else if (dynamic_cast<Bridge*>(previous) != nullptr) removeBridge(position);
	else if (dynamic_cast<Note*>(previous) != nullptr) removeNote(position);
}

void Grid::update()
{
	for (shared_ptr<Gate>& gate : gates) gate->update();
	for (shared_ptr<Wire>& wire : wires) wire->update();
}

uint8_t Grid::getId(const Int2& position) const
{
	shared_ptr<Cell> ptr = getPtr(position);
	return ptr ? ptr->getCellId() : 0u;
}

void Grid::setId(const Int2& position, const uint8_t& id)
{
	switch (id)
	{
		case 0: remove(position); return;
		case 5: addGate(position); return;
		case 6: addBridge(position); return;
		case 7: addNote(position); return;
		case 1: addWire(position); return;
		case 2: addWire(position); break;
		case 3: addWire(position); break;
		case 4: addWire(position); break;
		default: return;
	}

	Wire* wire = dynamic_cast<Wire*>(get(position));
	if (wire == nullptr) throw exception("Invalid");

	wire->process(id);
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

void Grid::removeBridge(const Int2& position)
{
	set(position, nullptr);
	splitNeighbors(position);
}

void Grid::removeNote(const Int2& position)
{
	set(position, nullptr);
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

		//The the current cell is a Bridge, we move
		//one block further to check for wires

		Bridge* bridge = dynamic_cast<Bridge*>(neighbor.get());

		if (bridge != nullptr)
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
/// Updates either the gate or bridge at position
/// by scanning its neighbors/surroundings
/// </summary>
void Grid::scanPort(const Int2& position)
{
	Port* port = dynamic_cast<Port*>(get(position));
	if (port == nullptr) return;

	Gate* const gate = dynamic_cast<Gate*>(port);
	Bridge* const bridge = dynamic_cast<Bridge*>(port);

	if (gate != nullptr) gate->refresh(*this, position);
	if (bridge != nullptr) bridge->refresh(*this, position);
}