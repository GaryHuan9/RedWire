#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

namespace RedWire
{
	int main();

	struct Application : RenderWindow
	{
		Application();

		void start();

		void dispatchEvents();

		void update();
	};
}