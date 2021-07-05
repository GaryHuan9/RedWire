#pragma once

#include "Cell.h"

namespace RedWire
{
	struct Port : Cell
	{
		Port();

		bool getEnabled() const;
		void setEnabled(const bool enabled);

	private:
		bool enabled;
	};
}