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
		enum class Mode
		{
			circuit,
			clipboard
		};

		enum class FileMode
		{
			load,
			save,
			remove
		};

		Mode mode = Mode::circuit;
		FileMode fileMode{};

		//finalized message
		std::string message;

		bool confirming{ false };
	};
}