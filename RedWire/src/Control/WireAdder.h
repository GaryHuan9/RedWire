#pragma once

#include "../Type2.h"
#include "LineTool.h"
#include "InputManager.h"

namespace RedWire
{
	struct WireAdder : LineTool
	{
		WireAdder(InputManager& manager);

		bool activationPredicate() override;

		void setLineCell(const Int2& cell) override;

		virtual void showUI() override;

	private:
		bool addSource{};
	};
}