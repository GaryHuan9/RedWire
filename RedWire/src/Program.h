#pragma once

namespace RedWire
{
	using namespace sf;

	struct Application : RenderWindow
	{
		Application();

		void start();

		void dispatchEvents();

		void update();
	};
}