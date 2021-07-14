#pragma once

#include <memory>
#include <array>

#include "Section.h"
#include "UIWindow.h"

namespace RedWire
{
	struct Application;

	//Should I inherit tool box with UIWindow since it's technically a window
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