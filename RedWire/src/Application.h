#pragma once

#include <SFML/System.hpp>
#include "Core/Grid.h"
#include "Core/GridViewNew.h"
#include "Core/InputManager.h"
#include "Core/TestUI.h"

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
		GridViewNew gridView;
		TestUI testUI;

	private:

		sf::Clock clock;
	};
}