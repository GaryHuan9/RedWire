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

		bool writeTo(std::ostream& stream);
		void readFrom(std::istream& stream);

	private:
		void updatePreview();

		enum Mode
		{
			copy,
			cut,
			paste
		};

		int mode = Mode::copy;

		Int2 startCell;
		Int2 lastCell;
		Int2 copiedSize;

		bool isCopying{};

		std::unique_ptr<Grid> previewGrid;
		std::unique_ptr<std::stringstream> buffer; //Because this stream could potentially be very larger, we put it on the heap
	};
}