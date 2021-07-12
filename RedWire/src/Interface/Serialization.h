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
		const std::string defaultSaveDirectory = "saves/";

		int mode = 0;

		//finalized message
		std::string message;
		std::array<char, 100ull> fileName;
	};
}