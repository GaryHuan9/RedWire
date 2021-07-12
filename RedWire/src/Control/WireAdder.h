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

		void setCell(const Int2& cell) override;

		virtual void showUI() override;

	private:

		enum Mode
		{
			unchanged,
			unpowered,
			powered
		};

		int mode = Mode::unchanged;
	};
}