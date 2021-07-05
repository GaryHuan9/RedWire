#pragma once

#include "Cell.h"

namespace RedWire
{
	struct Join : Cell
	{
		Join();

		uint32_t getColor() const override;
	};
}