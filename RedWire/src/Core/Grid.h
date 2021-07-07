#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include "../Type2.h"
#include "Area.h"
#include "Join.h"

namespace RedWire
{
	struct Cell;
	struct Wire;
	struct Gate;
	struct Region;

	struct Grid : Area
	{
		Grid();

		inline size_t getWiresCount() const { return wires.size(); }
		inline size_t getGatesCount() const { return gates.size(); }

		void addWire(const Int2& position);
		void addGate(const Int2& position);
		void addJoin(const Int2& position);

		void add(const Int2& position, const uint8_t& id);

		void remove(const Int2& position);
		void toggleSource(const Int2& position);

		void update();

	private:

		friend void Join::refresh(Grid& grid, const Int2& position);

		struct SearchPack
		{
			SearchPack(Wire* const wire, std::unordered_set<Int2>& replaced, const std::shared_ptr<Cell>& bundle) : wire(wire), replaced(replaced), bundle(bundle)
			{}

			Wire* const wire;

			std::unordered_set<Int2>& replaced;
			const std::shared_ptr<Cell>& bundle;
		};

		std::unordered_set<Int2> floodReplace(const Int2& position, const std::shared_ptr<Cell>& bundle);
		void floodSearch(const SearchPack& pack, const Int2& source, const Int2& direction);

		bool splitNeighbors(const Int2& position);
		void scanPort(const Int2& position);

		void removeWire(const Int2& position);
		void removeGate(const Int2& position);
		void removeJoin(const Int2& position);

		template<class Type> static void removeFrom(std::vector<std::shared_ptr<Type>>& vector, Type* target);

		std::vector<std::shared_ptr<Wire>> wires;
		std::vector<std::shared_ptr<Gate>> gates;
	};
}