#pragma once

#include "Section.h"

namespace RedWire
{
	struct Toolbox;

	struct General : Section
	{
		General(Toolbox& toolbox);

		virtual void show() override;

	private:
		void showTickControls();

		enum TickMode
		{
			perSecond,
			perFrame,
			manual
		};
	};
} 