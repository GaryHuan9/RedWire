#include "TextProjector.h"

#include "../Core/Region.h"
#include "../Type2.h"

#include <string>
#include <memory>
#include <fstream>
#include <stdint.h>
#include <unordered_map>

using namespace RedWire;

TextProjector::TextProjector(char* fileName)
{
	std::ifstream stream(fileName);

	if (stream.fail()) throw std::exception("Failed to open path!");

	char token;
	uint32_t nextWidth{};

	while (stream >> token)
	{
		stream.seekg(stream.tellg() + (std::streampos)1); //Eat the space

		switch (std::tolower(token))
		{
			case '#': continue; //Ignore comments
			case 'h': stream >> lineHeight; break;
			case 'w': stream >> nextWidth;  break;
			case 'c':
			{
				char character;
				stream >> character;

				Int2 size((int32_t)nextWidth, (int32_t)lineHeight);
				map.emplace(character, Glyph(stream, size));

				break;
			}
		}
	}
}


Region TextProjector::fromText(std::string text) const
{
	//const Int2 resultSize = SINGLE_CHAR_SIZE * size + SINGLE_CHAR_SIZE.x * (text.size() - 1ull);

	//Region result(resultSize);

	////assign and combine the region

	//for (const char& letter : text)
	//{
	//	//do stuff
	//}

	//return result;

	throw nullptr;
}

TextProjector::Glyph::Glyph(std::istream& stream, const Int2& size) : width((uint32_t)size.x)
{
	for (int32_t y = 0; y < size.y; y++)
	{
		for (int32_t x = 0; x < size.x; x++)
		{
			int value;
			stream >> value;

			//TODO
		}
	}
}

bool TextProjector::Glyph::get(const Int2& position) const
{
	return pixels[(size_t)position.y * width + position.x];
}
