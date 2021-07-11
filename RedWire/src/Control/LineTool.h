#pragma once

#include "Tool.h"

namespace RedWire
{
	struct LineTool : Tool
	{
		LineTool(InputManager& manager);

		virtual void showUI() override;

	protected:
		void doLineDraw(const bool& down, const Int2& cell);

		virtual void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		virtual void setLineCell(const Int2& cell) = 0;

	private:
		bool positionSet = false;
		bool directionSet = false;
		bool horizontal = false;

		Int2 startCell;
		Int2 lineMinMax;
	};
}


