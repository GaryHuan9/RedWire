#include "AreaSaver.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

using namespace RedWire;

AreaSaver::AreaSaver(InputManager& manager) : Tool(manager)
{}

void AreaSaver::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (down && changed) grid.writeTo("area");
}

bool AreaSaver::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::X);
}
