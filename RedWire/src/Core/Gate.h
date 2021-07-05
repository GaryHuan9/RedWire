#pragma once

namespace RedWire
{
	struct Grid;
	struct Port;

	struct Gate : Port
	{
		Gate();

		uint32_t getColor() const override;

		void setPositions(const Int2& source, const Int2& target, const Int2& control);

		void update(const Grid& grid) const;

	private:

		Int2 sourcePosition;
		Int2 targetPosition;
		Int2 controlPosition;
	};
}