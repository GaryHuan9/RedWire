#pragma once

namespace RedWire
{
	struct Application;

	struct UserInterface
	{
		UserInterface(Application& application);

		Application& application;

		virtual void update() = 0;
	};
}