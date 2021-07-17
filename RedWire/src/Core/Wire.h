#pragma once

#include "Cell.h"
#include <stdint.h>

namespace RedWire
{
	struct SourceTool;

	struct Wire : Cell
	{
		Wire();

		bool isSource{ false };

		uint32_t getColor() const override;
		uint8_t getCellId() const override;

		bool getPowered() const;
		void setPowered(const bool value);

		void update();

		void combine(const Wire& wire);
		void process(const uint8_t& id);

	private:
		friend struct SourceTool;

		bool poweredLast{ false };
		bool poweredNext{ false };

#ifdef _DEBUG
		uint32_t debugColor;
#endif
	};
}