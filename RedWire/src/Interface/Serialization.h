#pragma once

#include "Section.h"
#include <string>
#include <filesystem>

namespace RedWire
{
	struct Toolbox;

	struct Serialization : Section
	{
		Serialization(Toolbox& toolbox);

		virtual void show() override;

	private:
		const std::filesystem::path savePath = "saves/";

		enum class Mode
		{
			circuit,
			clipboard
		};

		Mode mode = Mode::circuit;

		//finalized message
		std::string message;
		std::array<char, 100ull> fileName;
	};
}