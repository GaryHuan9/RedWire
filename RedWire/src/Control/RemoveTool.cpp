#include "RemoveTool.h"
#include "../Application.h"
#include "../Interface/GridView.h"
#include "../Core/Grid.h"

#include <iostream>
#include <stdint.h>
#include "imgui.h"

using namespace RedWire;

RemoveTool::RemoveTool(InputManager& manager) : Tool(manager)
{}

void RemoveTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	Int2 oldCell = lastCell;
	lastCell = cell;

	if (!changed)
	{
		if (lastCell != oldCell) updatePreview();
		return;
	}

	bool express = InputManager::isPressed(sf::Keyboard::LShift);

	if (!started && down)
	{
		started = true;
		startCell = cell;
	}
	else if (started && (down || express))
	{
		Int2 min = startCell.min(cell);
		Int2 max = startCell.max(cell);

		for (int32_t y = min.y; y <= max.y; y++)
		{
			for (int32_t x = min.x; x <= max.x; x++)
			{
				grid->remove(Int2(x, y));
			}
		}

		started = false;
	}

	updatePreview();
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

void RemoveTool::showHelpUI()
{
	ImGui::Text("The RemoveTool removes a rectangular area of cells");
	ImGui::Text("Click on two corners to remove all cells underneath");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Keyboard Shortcut: Q");
}

void RemoveTool::updatePreview()
{
	GridView& view = manager.application.find<GridView>();

	static const uint32_t color = 0xFF030122u;

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
