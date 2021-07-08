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
	if (!down)
	{
		positionSet = false;
		directionSet = false;
	}
	else
	{
		if (!positionSet)
		{
			startCell = cell;
			positionSet = true;

			//incase you wanna click set, instead of drag set
			grid->addWire(cell);
		}

		//if not moved
		if (startCell == cell) return;

		if (!directionSet)
		{
			directionSet = true;

			Int2 direction = cell - startCell;
			horizontal = direction.x != 0;
		}

		if (horizontal)
		{
			int min = std::min(startCell.x, cell.x);
			int max = std::max(startCell.x, cell.x);

			for (int x = min; x <= max; x++) grid->addWire(Int2(x, startCell.y));
		}
		else
		{
			int min = std::min(startCell.y, cell.y);
			int max = std::max(startCell.y, cell.y);

			for (int y = min; y <= max; y++) grid->addWire(Int2(startCell.x, y));
		}
	}
}

bool WireAdder::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::E);
}
