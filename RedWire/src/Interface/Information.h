#pragma once

#include "Section.h"

namespace RedWire
{
	struct Toolbox;

	struct Information : Section
	{
		Information(Toolbox& toolbox);

		virtual void show() override;
	};
}