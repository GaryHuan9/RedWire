#pragma once

#include "../Type2.h"
#include "LineTool.h"
#include "InputManager.h"

namespace RedWire
{
	struct WireAdder : LineTool
	{
		WireAdder(InputManager& manager);

		bool activationPredicate() override;

		void SetLineCell(const Int2& cell) override;
	};
}