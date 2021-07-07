#pragma once

#include <SFML/System.hpp>
#include "Core/Grid.h"
#include "Control/InputManager.h"
#include "Interface/GridView.h"
#include "Interface/TestUI.h"

namespace RedWire
{
	struct Application : sf::RenderWindow
	{
		Application();

		void start();

		void dispatchEvents();

		void update();

		Grid grid;

		InputManager inputManager;
		GridView gridView;
		TestUI testUI;

	private:

		sf::Clock clock;
	};
}