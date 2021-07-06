#pragma once

#include "Port.h"

namespace RedWire
{
	struct Grid;

	struct Gate : Port
	{
		Gate();

		uint32_t getColor() const override;

		void refresh(Grid& grid, const Int2& position) override;

		void update() const;

	private:

		Grid* grid;

		Int2 sourcePosition;
		Int2 targetPosition;
		Int2 controlPosition;
	};
}