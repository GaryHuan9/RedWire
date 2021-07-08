#include "GateAdder.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

using namespace RedWire;

GateAdder::GateAdder(InputManager& manager) : Tool(manager)
{}

void GateAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (down && changed) grid.addGate(cell);
}

bool GateAdder::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
}
