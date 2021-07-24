#pragma once

#include <memory>
#include <array>

#include "Section.h"
#include "Window/UIWindow.h"

namespace RedWire
{
	struct Application;

	struct Toolbox : UIWindow
	{
		Toolbox(Application& application);

		inline void resetWindow() { resetWindowOrientation = true; }

	protected:
		void showUI() override;

	private:

		bool resetWindowOrientation{ true };
		std::array<std::unique_ptr<Section>, 4> sections;
	};
}