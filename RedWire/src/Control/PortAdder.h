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

		void showHelpUI() override;

	private:
		enum class Mode
		{
			gate,
			bridge
		};

		Mode mode = Mode::gate;
		bool overrideCell{ true };
	};
}