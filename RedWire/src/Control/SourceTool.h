#pragma once

#include "../Type2.h"
#include "Tool.h"

namespace RedWire
{
	struct InputManager;

	struct SourceTool : Tool
	{
		SourceTool(InputManager& manager);

		void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) override;

		bool activationPredicate() override;

		void showUI() override;

		void showHelpUI() override;

	private:
		enum class Mode
		{
			toggle,
			power,
			unpower
		};

		enum class Type
		{
			permanent,
			temporary,
			disabled
		};

		Mode mode = Mode::toggle;
		Type type = Type::permanent;
	};
}