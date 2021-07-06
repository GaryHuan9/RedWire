#pragma once

#include "Port.h"

namespace RedWire
{
	struct Grid;

	struct Join : Port
	{
		Join();

		uint32_t getColor() const override;

		void refresh(Grid& grid, const Int2& position) override;
	};
}