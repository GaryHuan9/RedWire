#pragma once

#include "Port.h"
#include <memory>

namespace RedWire
{
	struct Grid;
	struct Wire;

	struct Junction : Port
	{
		void refresh(Grid& grid, const Int2& position) override;

	protected:
		const std::shared_ptr<Cell>* sourceWire = nullptr;
		const std::shared_ptr<Cell>* targetWire = nullptr;
		const std::shared_ptr<Cell>* controlWire = nullptr;

	private:
		Grid* grid{};
	};
}