#pragma once

#include "Cell.h"

namespace RedWire
{
	struct Junction : Cell
	{
		Junction();

		uint32_t getColor() const override;
	};
}