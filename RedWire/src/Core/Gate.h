#pragma once

#include "Junction.h"
#include <memory>

namespace RedWire
{
	struct Grid;
	struct Wire;

	struct Gate : Junction
	{
		uint32_t getColor() const override;
		uint8_t getCellId() const override;

		void update() const;
	};
}