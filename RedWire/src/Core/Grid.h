#pragma once

#include <unordered_map>
#include <memory>
#include "Cell.h"
#include "Int2.h"

namespace RedWire
{
	struct Grid
	{
		struct Tile
		{
			static const int size = 32;

			std::shared_ptr<Cell> cells[size][size];
		};

		Grid();

		const Cell& operator [] (const Int2& position) const;
		Cell& operator [] (const Int2& position);

	private:
		const std::unordered_map<Int2, Tile> tiles;
	};
}