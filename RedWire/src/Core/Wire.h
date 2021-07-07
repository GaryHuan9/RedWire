#pragma once

#include <stdint.h>
#include "Cell.h"

namespace RedWire
{
	struct Wire : Cell
	{
		Wire();

		bool isSource;

		uint32_t getColor() const override;
		uint8_t getCellId() const override;

		bool getPowered() const;
		void setPowered(const bool powered);

		void update();

		void combine(const Wire& wire);
		void process(const uint8_t& id);

	private:

		bool poweredLast;
		bool poweredNext;

		uint32_t debugColor;
	};
}