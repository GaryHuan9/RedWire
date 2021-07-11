#include "WireAdder.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

#include <iostream>

using namespace RedWire;

WireAdder::WireAdder(InputManager& manager) : LineTool(manager)
{}

bool WireAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::E);
}

void WireAdder::setLineCell(const Int2& cell)
{
	grid->addWire(cell);
}
