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
		void setMessage(const char* text);

		static const float MESSAGE_DISPLAY_SPAN; // second

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

		float messageTimeSpan = 0.0f;

		bool messageSet{ false };
		bool confirming{ false };
	};
}