#pragma once

#include <stdint.h>

namespace RedWire
{
	struct Cell
	{
		/// <summary>
		/// Should return the RGBA value of this cell.
		/// </summary>
		virtual uint32_t getColor() const = 0;

		/// <summary>
		/// Should return a 4 bit number that can be used to
		/// identify the type and current state of this cell.
		/// </summary>
		virtual uint8_t getCellId() const = 0;
	};
}