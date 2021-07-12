#pragma once

#include <unordered_map>
#include <fstream>
#include <memory>
#include "../Type2.h"

namespace RedWire
{
	struct Cell;
	struct Grid;

	struct Area
	{
		Area();

		inline size_t getTileCount() const { return tiles.size(); }

		Cell* const get(const Int2& position) const;
		uint32_t getColor(const Int2& position) const;

		void writeTo(std::ostream& stream, const Int2& min, const Int2& max) const;
		void writeTo(std::ostream& stream, const Float2& viewCenter, const float& viewExtend) const;

		static void readFrom(std::istream& stream, Grid& grid, const Int2& min);
		static std::unique_ptr<Grid> readFrom(std::istream& stream);
		static std::unique_ptr<Grid> readFrom(std::istream& stream, Float2& viewCenter, float& viewExtend);

		struct Tile
		{
			Tile();

			static const size_t size = 32;

			/// <summary>
			/// The number of cells held by this tile. This number should not be larger than size * size.
			/// If this number is zero, then Area should remove this tile from the heap memory entirely.
			/// </summary>
			size_t cellCount{ 0 };

			std::shared_ptr<Cell> cells[size][size];
		};

	protected:

		static Int2 getTilePosition(const Int2& position);

		std::shared_ptr<Cell> getPtr(const Int2& position) const;
		void set(const Int2& position, const std::shared_ptr<Cell> cell);

	private:

		void findBorder(Int2& min, Int2& max) const;

		std::unordered_map<Int2, Tile> tiles;
	};
}