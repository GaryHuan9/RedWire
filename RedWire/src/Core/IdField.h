#pragma once

#include "../Type2.h"
#include <cstdint>

#include <istream>
#include <ostream>

namespace RedWire
{
	struct IdField
	{
		virtual uint8_t getId(const Int2& position) const = 0;
		virtual void setId(const Int2& position, const uint8_t& id) = 0;

		void writeTo(std::ostream& stream, const Int2& min, const Int2& max) const;

		static Int2 readFrom(std::istream& stream, IdField& field);
		static Int2 readFrom(std::istream& stream, IdField& field, const Int2& destination);

	protected:
		static Int2 readBorder(std::istream& stream, Int2& min, Int2& max); //Returns the size
		static void readField(std::istream& stream, IdField& field, const Int2& size, const Int2& offset);
	};
}