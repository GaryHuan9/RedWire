#pragma once

#include "../Type2.h"
#include "Tool.h"
#include "InputManager.h"

namespace RedWire
{
	struct WireAdder : Tool
	{
		WireAdder(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

	private:
		bool positionSet;
		bool directionSet;
		Int2 cellPosition;
		bool horizontal;
	};
}