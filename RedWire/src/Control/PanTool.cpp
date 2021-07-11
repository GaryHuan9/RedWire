#include "PanTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Application.h"
#include "../Type2.h"
#include "../Core/Grid.h"
#include <SFML/System.hpp>

using namespace RedWire;

PanTool::PanTool(InputManager& manager) : Tool(manager)
{}

void PanTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (changed)
	{
		//Check toggle source
		static const sf::Time maxDelay = sf::seconds(0.3f);
		sf::Time time = manager.application.getTotalTime();

		if (down) pressedTime = time;
		else if (time - pressedTime < maxDelay)
		{
			grid->setSource(cell, !grid->getSource(cell));
		}

		//Calculate panning
		if (down) pressPosition = position;
	}
	else if (down) manager.viewCenter += pressPosition - position;
}

bool PanTool::activationPredicate()
{
	return InputManager::isPressed(sf::Mouse::Right);
}
