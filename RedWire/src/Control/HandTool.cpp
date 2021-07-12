#include "HandTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Application.h"
#include "../Type2.h"
#include "../Core/Grid.h"
#include <SFML/System.hpp>

using namespace RedWire;

HandTool::HandTool(InputManager& manager) : Tool(manager)
{}

void HandTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!changed) return;

	//Check toggle source
	static const sf::Time maxDelay = sf::seconds(0.3f);
	sf::Time time = manager.application.getTotalTime();

	if (down) pressedTime = time;
	else if (time - pressedTime < maxDelay)
	{
		grid->setSource(cell, !grid->getSource(cell));
	}
}

bool HandTool::activationPredicate()
{
	return InputManager::isPressed(sf::Mouse::Right);
}
