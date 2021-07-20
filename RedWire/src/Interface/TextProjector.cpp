#include "TextProjector.h"

#include "../Core/Region.h"
#include "../Core/Note.h"
#include "../Type2.h"

#include <iostream>

#include <string>
#include <memory>
#include <fstream>
#include <stdint.h>
#include <unordered_map>

using namespace RedWire;

TextProjector::TextProjector(const char* fileName)
{
	std::ifstream stream(fileName);

	if (stream.fail()) throw std::exception("Failed to open path!");

	char token;
	uint32_t nextWidth{};

	while (stream >> token)
	{
		stream.seekg(stream.tellg() + (std::streampos)1); //Eat the space, delicious!

		switch (std::tolower(token))
		{
			case '#': continue; //Ignore comments
			case 'h': stream >> lineHeight; break;
			case 'w': stream >> nextWidth;  break;
			case 'c':
			{
				char character;
				stream >> character;

				Int2 size = Int2((int32_t)nextWidth, (int32_t)lineHeight);
				//NOTE: Stored as lower case
				map.emplace(std::tolower(character), Glyph(stream, size));

				break;
			}
		}
	}

	//debug ->
	//for (auto& item : map) std::cout << item.first << ": " << item.second.width << '\n';

	stream.close();
}

Region TextProjector::fromText(std::string text) const
{
	//NOTE: remember to make it lower case
	int32_t totalWidth = static_cast<int32_t>(text.length() - 1ull); //Spaces between letters
	for (auto& letter : text) totalWidth += map.at(std::tolower(letter)).width;

	Grid grid;
	int32_t cursor{ 0 };

	for (auto& letter : text)
	{
		const char lower = std::tolower(letter);

		// checks if exists
		if (map.find(lower) == map.end()) continue;

		const Glyph& glyph = map.at(lower);

		for (uint32_t y = 0u; y < lineHeight; y++)
		{
			for (uint32_t x = 0u; x < glyph.width; x++)
			{
				const Int2 offset(x, y);

				if (!glyph.get(offset)) continue;

				grid.addNote(Int2(cursor + x, y));
			}
		}

		cursor += glyph.width + 1;
	}

	Region region(Int2(totalWidth, lineHeight));
	region.copyFrom(Int2(0, 0), grid);

	return region;
}

static size_t getLength(const Int2& size)
{
	return (size_t)size.x * size.y;
}

TextProjector::Glyph::Glyph(std::istream& stream, const Int2& size) : width((uint32_t)size.x), pixels(std::make_unique<bool[]>(getLength(size)))
{
	for (int32_t y = 0; y < size.y; y++)
	{
		for (int32_t x = 0; x < size.x; x++)
		{
			int value{ 0 };
			stream >> value;

			pixels[getIndex(Int2(x, y))] = value != 0;
		}
	}
}

bool TextProjector::Glyph::get(const Int2& position) const
{
	return pixels[getIndex(position)];
}

size_t TextProjector::Glyph::getIndex(const Int2& position) const
{
	return (size_t)position.y * width + position.x;
}
