#include "WireAdder.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

using namespace RedWire;

WireAdder::WireAdder(InputManager& manager) : Tool(manager), positionSet(false), directionSet(false), cellPosition(0u, 0u), horizontal(false)
{}

void WireAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!down)
	{
		positionSet = false;
		directionSet = false;
	}
	else
	{
		if (!positionSet)
		{
			cellPosition = cell;
			positionSet = true;
		}

		if (!directionSet)
		{
			//if not moved
			if (cellPosition == cell) return;

			//else

			directionSet = true;

			Int2 difference = cell - cellPosition;
			horizontal = difference.x != 0;
		}

		Int2 targetCell(horizontal ? cell.x : cellPosition.x, horizontal ? cellPosition.y : cell.y);

		grid.addWire(targetCell);
	}
}

bool WireAdder::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::E);
}
