#pragma once

#include "Section.h"
#include <string>

namespace RedWire
{
	struct Toolbox;

	struct Serialization : Section
	{
		Serialization(Toolbox& toolbox);

		virtual void show() override;

	private:
		const char* defaultSaveDirectory = "saves";

		int mode = 0;
		//finalized message
		std::string message;
		std::array<char, 100ull> filePath;
		//char filePath[100] = "File path here";
	};
}