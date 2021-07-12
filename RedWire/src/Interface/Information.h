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
		const float FPS_UPDATE_INTERVAL = 1.f;

		float currentInterval = 0.f;
		float lastAverageFPS = 0.f;
		int frameCount = 0;
	};
}