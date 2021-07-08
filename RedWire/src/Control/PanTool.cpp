#include "PanTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

using namespace RedWire;

PanTool::PanTool(InputManager& manager) : Tool(manager)
{}

void PanTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	//View panning
	//Wire toggling

	if (down && changed) grid.toggleSource(cell);
}

bool PanTool::activationPredicate()
{
	return sf::Mouse::isButtonPressed(sf::Mouse::Right);
}
