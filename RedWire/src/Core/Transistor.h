#pragma once

#include "Gate.h"
#include <stdint.h>

namespace RedWire
{
	struct Transistor : Gate
	{
		uint32_t getColor() const override;
		uint8_t getCellId() const override;

		void update() const;
	};
}