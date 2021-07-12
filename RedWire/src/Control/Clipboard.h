#pragma once

#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"
#include <sstream>
#include <memory>

namespace RedWire
{
	struct Clipboard : Tool
	{
		Clipboard(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		void onDisable() override;

		bool activationPredicate() override;

		void showUI() override;

	private:
		void updatePreview();

		Int2 startCell;
		Int2 lastCell;
		Int2 copiedSize;

		bool isCopying{};

		int mode{}; //0 is copy, 1 is paste

		std::unique_ptr<Grid> previewGrid;
		std::unique_ptr<std::stringstream> stream; //Because this stream could poptentially be very larger, we put it on the heap
	};
}