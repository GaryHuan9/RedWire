#pragma once

namespace RedWire
{
	using namespace sf;

	int main();

	struct Application : RenderWindow
	{
		Application();

		void start();

		void dispatchEvents();

		void update();
	};
}