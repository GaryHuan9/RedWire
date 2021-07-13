#include "RemoveTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Application.h"
#include "../Interface/GridView.h"
#include "../Type2.h"
#include "../Core/Grid.h"

#include <iostream>
#include "imgui.h"

using namespace RedWire;

RemoveTool::RemoveTool(InputManager& manager) : Tool(manager)
{}

void RemoveTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	Int2 old = lastCell;
	lastCell = cell;

	if (changed && down)
	{
		if (started)
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
		else
		{
			started = true;
			startCell = cell;
		}

		updatePreview();
	}
	else if (lastCell != old) updatePreview();
}

void RemoveTool::onDisable()
{
	started = false;
	Tool::onDisable();
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

	Int2 delta = max - min + Int2(1);
	ImGui::Text("Removing %u x %u", delta.x, delta.y);
}

void RemoveTool::doHelpUI()
{
	ImGui::Text("This can remove a rectangular area of cells\nTo use it, click on two corner cells to remove a rectangular area.");
}

void RemoveTool::updatePreview()
{
	GridView& view = manager.application.find<GridView>();

	static const uint32_t color = 0xFF111113u;

	if (started)
	{
		Int2 min = startCell.min(lastCell);
		Int2 max = startCell.max(lastCell);

		Int2 size = max - min + Int2(1);

		view.setPreviewMin(min);
		view.setPreviewSize(size);

		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
				view.setPreviewColor(Int2(x, y), color);
			}
		}
	}
	else
	{
		view.setPreviewMin(lastCell);
		view.setPreviewSize(Int2(1));

		view.setPreviewColor(Int2(0), color);
	}
}
