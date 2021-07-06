#pragma once

#include <stdint.h>
#include <memory>
#include "Type2.h"

namespace RedWire
{
	struct Area;

	struct Region
	{
		Region(const Int2& size);

		const Int2 size;

		void copyFrom(const Area& area, const Int2& position);

	private:

		size_t getIndex(const Int2& position) const;

		const uint8_t& get(const Int2& position) const;
		void set(const Int2& position, const uint8_t& byte);

		std::unique_ptr<uint8_t[]> bytes;
	};
}