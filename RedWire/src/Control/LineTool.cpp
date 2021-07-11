#include "LineTool.h"

using namespace RedWire;

LineTool::LineTool(InputManager& manager) : Tool(manager) {}

void LineTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	DoLineDraw(down, cell);
}

void LineTool::DoLineDraw(const bool& down, const Int2& cell)
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
		//incase you wanna click set, instead of drag set
		SetLineCell(cell);
		return;
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

		for (int x = min; x <= max; x++) SetLineCell(Int2(x, startCell.y));
	}
	else
	{
		int min = std::min(startCell.y, cell.y);
		int max = std::max(startCell.y, cell.y);

		for (int y = min; y <= max; y++) SetLineCell(Int2(startCell.x, y));
	}
}
