#pragma once

#include "Port.h"

namespace RedWire
{
	struct Grid;

	struct Bridge : Port
	{
		Bridge();

		uint32_t getColor() const override;
		uint8_t getCellId() const override;

		void refresh(Grid& grid, const Int2& position) override;
	};
}