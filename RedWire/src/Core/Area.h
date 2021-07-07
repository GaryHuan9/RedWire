#pragma once

#include <unordered_map>
#include <memory>
#include "../Type2.h"

namespace RedWire
{
	struct Cell;

	struct Area
	{
		struct Tile
		{
			Tile();

			static const int size = 32;

			std::shared_ptr<Cell> cells[size][size];
		};

		Area();

		Cell* const get(const Int2& position) const;
		uint32_t getColor(const Int2& position) const;

	protected:

		std::shared_ptr<Cell> getPtr(const Int2& position) const;
		void set(const Int2& position, const std::shared_ptr<Cell> cell);

		std::unordered_map<Int2, Tile> tiles;
	};
}