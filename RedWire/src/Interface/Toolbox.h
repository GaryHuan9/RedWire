#pragma once

#include <memory>
#include <array>

#include "Section.h"
#include "../Component.h"

namespace RedWire
{
	struct Application;

	struct Toolbox : Component
	{
		Toolbox(Application& application);

		virtual void update() override;

		inline void resetWindow() { resetWindowOrientation = true; }

	private:

		bool resetWindowOrientation{ true };
		std::array<std::unique_ptr<Section>, 4> sections;
	};
}