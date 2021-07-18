#pragma once

#include "../Core/Region.h"

#include <string>

namespace RedWire
{
	struct TextProjector
	{
	public:
		TextProjector();

		Region FromText(std::string text, uint32_t size) const;

	private:
		static const Int2 SINGLE_CHAR_SIZE;
	};
}
