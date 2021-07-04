#pragma once

#include <stdint.h>

namespace RedWire
{
	struct Cell
	{
		virtual uint32_t getColor() const = 0;
	};
}