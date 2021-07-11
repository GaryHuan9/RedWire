#pragma once

#include "LineTool.h"

namespace RedWire
{
	struct NoteAdder : LineTool
	{
		NoteAdder(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

		void SetLineCell(const Int2& cell) override;
	};
}

