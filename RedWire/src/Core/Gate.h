#pragma once

#include "Port.h"
#include <memory>

namespace RedWire
{
	struct Grid;
	struct Cell;
	struct Wire;

	/// <summary>
	/// A Port that takes in exactly 3 inputs that forms a T shape.
	/// </summary>
	struct Gate : Port
	{
		void refresh(Grid& grid, const Int2& position) override;

	protected:
		const std::shared_ptr<Cell>* wire0 = nullptr;
		const std::shared_ptr<Cell>* wire1 = nullptr;
		const std::shared_ptr<Cell>* controlWire = nullptr;

	private:
		Grid* grid{};
	};
}