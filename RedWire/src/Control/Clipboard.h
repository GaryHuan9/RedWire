#pragma once

#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Region.h"

#include <istream>
#include <ostream>
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

		void readFrom(std::istream& stream);
		bool writeTo(std::ostream& stream);

		void showHelpUI() override;

	private:
		void updatePreview();

		enum class Mode
		{
			copy,
			cut,
			paste
		};

		Mode mode = Mode::copy;
		bool overrideCell{ true };

		Int2 startCell;
		Int2 lastCell;

		bool isCopying{};
		Region buffer;
	};
}