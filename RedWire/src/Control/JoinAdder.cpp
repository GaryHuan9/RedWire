#include "JoinAdder.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

using namespace RedWire;

JoinAdder::JoinAdder(InputManager& manager) : Tool(manager)
{}

void JoinAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (down && changed) grid->addJoin(cell);
}

bool JoinAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Num2);
}
