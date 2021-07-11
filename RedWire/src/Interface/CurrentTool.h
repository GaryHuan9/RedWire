#pragma once

#include "Section.h"

namespace RedWire
{
	struct Toolbox;

	struct CurrentTool : Section
	{
		CurrentTool(Toolbox& toolbox);

		virtual void show() override;
	};
}