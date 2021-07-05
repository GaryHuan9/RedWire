#pragma once

#include "Cell.h"
#include "Grid.h"

namespace RedWire
{
	struct Gate : Cell
	{
		Gate();

		uint32_t getColor() const override;

		void disable();

		void setPositions(const Int2& source, const Int2& target, const Int2& control);

		void update(const std::shared_ptr<Grid> grid) const;

	private:

		bool enabled;

		Int2 sourcePosition;
		Int2 targetPosition;
		Int2 controlPosition;
	};
}