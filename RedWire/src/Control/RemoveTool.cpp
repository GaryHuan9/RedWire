#include "RemoveTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

using namespace RedWire;

RemoveTool::RemoveTool(InputManager& manager) : Tool(manager)
{}

void RemoveTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (down) grid->remove(cell);
}

bool RemoveTool::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}
