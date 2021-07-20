#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include "../Type2.h"
#include "Area.h"
#include "Bridge.h"

namespace RedWire
{
	struct Region;
	struct Cell;
	struct Wire;
	struct Inverter;
	struct Transistor;

	struct Grid : Area
	{
		inline size_t getWireCount() const { return wires.size(); }
		inline size_t getInverterCount() const { return inverters.size(); }
		inline size_t getTransistorCount() const { return transistors.size(); }

		void addWire(const Int2& position);
		void addInverter(const Int2& position);
		void addTransistor(const Int2& position);
		void addBridge(const Int2& position);
		void addNote(const Int2& position);

		void remove(const Int2& position);

		void update();

	protected:

		friend struct Region;

		uint8_t getId(const Int2& position) const override;
		void setId(const Int2& position, const uint8_t& id) override;

	private:
		friend void Bridge::refresh(Grid& grid, const Int2& position);

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
		void removeInverter(const Int2& position);
		void removeTransistor(const Int2& position);
		void removeBridge(const Int2& position);
		void removeNote(const Int2& position);

		template<typename Type> static void removeFrom(std::vector<std::shared_ptr<Type>>& vector, Type* target);

		std::vector<std::shared_ptr<Wire>> wires;
		std::vector<std::shared_ptr<Inverter>> inverters;
		std::vector<std::shared_ptr<Transistor>> transistors;
	};
}