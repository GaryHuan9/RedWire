#pragma once

#include "UIWindow.h"

namespace RedWire
{
	struct Application;

	struct CreditWindow : UIWindow
	{
		CreditWindow(Application& application);

	protected:
		void showUI() override;
	};
}


