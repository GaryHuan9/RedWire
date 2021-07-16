#include "LineTool.h"
#include <cstdint>
#include "imgui.h"

using namespace RedWire;

LineTool::LineTool(InputManager& manager) : Tool(manager) {}

void LineTool::showUI()
{
	if (!directionSet) return;

	const char* label = horizontal ? "Horizontal" : "Vertical";
	const int32_t delta = lineMinMax.y - lineMinMax.x + 1;

	ImGui::Text("%s: %u", label, delta);
}

void LineTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!down)
	{
		positionSet = false;
		directionSet = false;

		return;
	}

	if (!positionSet)
	{
		startCell = cell;
		positionSet = true;
	}

	//If not moved
	if (startCell == cell && !directionSet)
	{
		setCell(cell);
		return;
	}

	if (!directionSet)
	{
		directionSet = true;
		lineMinMax = Int2(0);

		Int2 direction = cell - startCell;
		horizontal = direction.x != 0;
	}

	if (horizontal)
	{
		int min = std::min(startCell.x, cell.x);
		int max = std::max(startCell.x, cell.x);

		Int2 oldMinMax = lineMinMax;
		lineMinMax = Int2(min, max);
		if (oldMinMax == lineMinMax) return;

		for (int x = min; x <= max; x++) setCell(Int2(x, startCell.y));
	}
	else
	{
		int min = std::min(startCell.y, cell.y);
		int max = std::max(startCell.y, cell.y);

		Int2 oldMinMax = lineMinMax;
		lineMinMax = Int2(min, max);
		if (oldMinMax == lineMinMax) return;

		for (int y = min; y <= max; y++) setCell(Int2(startCell.x, y));
	}
}
