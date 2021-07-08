#include "WireAdder.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

using namespace RedWire;

WireAdder::WireAdder(InputManager& manager) : Tool(manager)
{}

void WireAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (down) grid.addWire(cell);
}

bool WireAdder::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::E);
}
