#pragma once

#include "../Type2.h"
#include "Tool.h"
#include "InputManager.h"
#include <SFML/System.hpp>

namespace RedWire
{
	struct PanTool : Tool
	{
		PanTool(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

	private:
		sf::Time pressedTime;
		Float2 pressPosition;
	};
}