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

		void showHelpUI() override;

	private:

		enum class Mode
		{
			unchanged,
			unpowered,
			powered
		};

		Mode mode = Mode::unchanged;
		bool overrideCell{ true };
		bool autoAddBridge{ false };
	};
}