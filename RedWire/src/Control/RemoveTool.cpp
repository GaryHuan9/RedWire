#include "RemoveTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

#include <iostream>
#include "imgui.h"

using namespace RedWire;

RemoveTool::RemoveTool(InputManager& manager) : Tool(manager)
{}

void RemoveTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	lastCell = cell;
	if (!changed) return;

	if (down)
	{
		started = true;
		startCell = cell;
	}
	else
	{
		Int2 min = startCell.min(cell);
		Int2 max = startCell.max(cell);

		for (int y = min.y; y <= max.y; y++)
		{
			for (int x = min.x; x <= max.x; x++)
			{
				grid->remove(Int2(x, y));
			}
		}

		started = false;
	}
}

bool RemoveTool::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Q);
}

void RemoveTool::showUI()
{
	if (!started) return;

	Int2 min = startCell.min(lastCell);
	Int2 max = startCell.max(lastCell);

	if (min == max) return;

	Int2 delta = max - min + Int2(1);

	ImGui::Text("%u x %u", delta.x, delta.y);
}
