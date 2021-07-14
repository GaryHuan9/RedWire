#pragma once

#include <SFML/System.hpp>

#include "../Type2.h"
#include "Tool.h"
#include "InputManager.h"

namespace RedWire
{
	struct SourceTool : Tool
	{
		SourceTool(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

		void showHelpUI() override;

	private:
		sf::Time pressedTime;
	};
}