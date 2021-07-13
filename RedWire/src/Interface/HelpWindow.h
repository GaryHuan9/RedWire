#pragma once

#include "UIWindow.h"

namespace RedWire
{
	struct Application;

	struct HelpWindow : UIWindow
	{
		HelpWindow(Application& application);

	protected:
		void doUI() override;

	private:
	};
}

