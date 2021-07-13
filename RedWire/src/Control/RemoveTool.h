#pragma once

#include "../Type2.h"
#include "Tool.h"
#include "InputManager.h"

namespace RedWire
{
	struct RemoveTool : Tool
	{
		RemoveTool(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		void onDisable() override;

		bool activationPredicate() override;

		virtual void showUI() override;

		void showHelpUI() override;

	private:

		void updatePreview();

		bool started{};

		Int2 lastCell;
		Int2 startCell;
	};
}