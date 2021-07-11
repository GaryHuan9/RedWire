#pragma once

namespace RedWire
{
	struct Application;

	struct Toolbox
	{
		Toolbox(Application& application);

		void update();

		Application& application;
	};
}