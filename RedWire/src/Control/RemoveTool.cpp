#include "RemoveTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

#include <iostream>

using namespace RedWire;

RemoveTool::RemoveTool(InputManager& manager) : Tool(manager)
{}

void RemoveTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!changed) return;

	if (down)
	{
		if (!startCellSet)
		{
			startCell = cell;
			startCellSet = true;
			return;
		}
	}
	else
	{
		startCellSet = false;

		Int2 min = startCell.min(cell);
		Int2 max = startCell.max(cell);

		for (int y = min.y; y <= max.y; y++)
		{
			for (int x = min.x; x <= max.x; x++)
			{
				grid->remove(Int2(x, y));
			}
		}
	}
}

bool RemoveTool::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Q);
}
