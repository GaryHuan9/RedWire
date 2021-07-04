#pragma once

#include "Cell.h"

namespace RedWire
{
	struct Wire : Cell
	{
		Wire();

		bool isSource;

		uint32_t getColor() const;

		bool getPowered() const;
		void setPowered(const bool powered);

		void update();

		void combine(const Wire& wire);

	private:

		bool poweredLast;
		bool poweredNext;
	};
}