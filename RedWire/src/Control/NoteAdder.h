#pragma once

#include "Tool.h"

namespace RedWire
{
	struct NoteAdder : Tool
	{
		NoteAdder(InputManager& manager);

		virtual void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		virtual bool activationPredicate() override;
	};
}

