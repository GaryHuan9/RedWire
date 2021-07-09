#pragma once

#include "../Type2.h"
#include "Tool.h"
#include "InputManager.h"
#include <sstream>
#include <memory>

namespace RedWire
{
	struct Clipboard : Tool
	{
		Clipboard(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

	private:
		Int2 startCell{};
		bool hasCopied{};

		std::unique_ptr<std::stringstream> stream; //Because this stream could poptentially be very larger, we put it on the heap
	};
}