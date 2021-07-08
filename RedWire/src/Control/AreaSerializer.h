#pragma once

#include "../Type2.h"
#include "Tool.h"
#include "InputManager.h"

namespace RedWire
{
	//NOTE: This is a temporary tool to test Area serialization!

	struct AreaSerializer : Tool
	{
		AreaSerializer(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

	private:
		Int2 startCell;
	};
}