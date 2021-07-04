#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "Cell.h"
#include "Int2.h"

namespace RedWire
{
	struct Wire;
	struct Gate;

	struct Grid
	{
		struct Tile
		{
			Tile();

			static const int size = 32;

			std::shared_ptr<Cell> cells[size][size];
		};

		Grid();

		Cell* const get(const Int2& position) const;
		uint32_t getColor(const Int2& position) const;

		void addWire(const Int2& position);
		void addGate(const Int2& position);

		void remove(const Int2& position);

	private:

		void set(const Int2& position, const std::shared_ptr<Cell> cell);

		std::unordered_map<Int2, Tile> tiles;
		std::vector<std::shared_ptr<Wire>> wires;
		std::vector<std::shared_ptr<Gate>> gates;
	};
}