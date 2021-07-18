#pragma once

#include "../Interface/TextProjector.h"
#include "../Type2.h"

#include "LineTool.h"
#include <string>

namespace RedWire
{
	struct InputManager;

	struct NoteAdder : LineTool
	{
		NoteAdder(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

		void setCell(const Int2& cell) override;

		virtual void showUI() override;

		void showHelpUI() override;

	private:
		bool drawLines{};
		bool pressed{};

		std::string inputText;
		TextProjector projector;
	};
}

