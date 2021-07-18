#pragma once

#include "Gate.h"
#include <memory>
#include <stdint.h>

namespace RedWire
{
	struct Inverter : Gate
	{
		uint32_t getColor() const override;
		uint8_t getCellId() const override;

		void update() const;
	};
}