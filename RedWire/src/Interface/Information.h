#pragma once

#include "Section.h"

namespace RedWire
{
	struct Toolbox;

	struct Information : Section
	{
		Information(Toolbox& toolbox);

		virtual void show() override;

	private:
		const float FPSUpdateInterval = 1.0f;

		float currentInterval = 0.0f;
		float lastAverageFPS = 0.0f;
		int frameCount = 0;
	};
}