#include "TextProjector.h"

using namespace RedWire;

const Int2 TextProjector::SINGLE_CHAR_SIZE = Int2(4, 5);

TextProjector::TextProjector() {}

Region TextProjector::FromText(std::string text, uint32_t size) const
{
	const Int2 resultSize = SINGLE_CHAR_SIZE * size + SINGLE_CHAR_SIZE.x * (text.size() - 1ull);

	Region result(resultSize);

	//assign and combine the region

	for (const char& letter : text)
	{
		//do stuff
	}

	return result;
}


