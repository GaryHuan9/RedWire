#pragma once

#include "Type2.h"
#include "Cell.h"

namespace RedWire
{
	struct Grid;

	struct Port : Cell
	{
		Port();

		bool getEnabled() const;
		void setEnabled(const bool enabled);

		virtual void refresh(Grid& grid, const Int2& position) = 0;

	private:
		bool enabled;
	};
}