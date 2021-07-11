#pragma once

#include "Cell.h"
#include <stdint.h>

namespace RedWire
{
	struct Note : Cell
	{
		uint32_t getColor() const override;
		uint8_t getCellId() const override;
	};
}


