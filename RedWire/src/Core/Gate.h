#pragma once

#include "Port.h"
#include <memory>

namespace RedWire
{
	struct Grid;

	struct Gate : Port
	{
		uint32_t getColor() const override;
		uint8_t getCellId() const override;

		void refresh(Grid& grid, const Int2& position) override;

		void update() const;

	private:
		Wire* const getWire(const Int2& position) const;

		Grid* grid{};

		const std::shared_ptr<Cell>* sourceWire = nullptr;
		const std::shared_ptr<Cell>* targetWire = nullptr;
		const std::shared_ptr<Cell>* controlWire = nullptr;
	};
}