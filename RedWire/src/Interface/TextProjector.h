#pragma once

#include "../Core/Region.h"
#include "../Type2.h"

#include <string>
#include <memory>
#include <stdint.h>
#include <unordered_map>

namespace RedWire
{
	struct TextProjector
	{
		TextProjector(const char* fileName);

		Region fromText(std::string text) const;

	private:
		struct Glyph
		{
			Glyph(std::istream& stream, const Int2& size);
			Glyph(const Int2& size);

			bool get(const Int2& position) const;

			const uint32_t width;

		private:
			size_t getIndex(const Int2& position) const;

			std::unique_ptr<bool[]> pixels;
		};

		uint32_t lineHeight{};
		std::unordered_map<char, Glyph> map;
	};
}
