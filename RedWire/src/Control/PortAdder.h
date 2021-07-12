#pragma once

#include "../Type2.h"
#include "Tool.h"
#include "InputManager.h"

namespace RedWire
{
	struct PortAdder : Tool
	{
		PortAdder(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

		virtual void showUI() override;

	private:

		enum Mode
		{
			gate,
			join
		};

		int mode = Mode::gate;
	};
}