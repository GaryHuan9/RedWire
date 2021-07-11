#pragma once

#include "Tool.h"

namespace RedWire
{
	struct LineTool : Tool
	{
		LineTool(InputManager& manager);

	protected:
		void DoLineDraw(const bool& down, const Int2& cell);

		virtual void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		virtual void SetLineCell(const Int2& cell) = 0;

		bool positionSet = false;
		bool directionSet = false;
		bool horizontal = false;
		Int2 startCell;
	};
}


