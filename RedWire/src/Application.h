#pragma once

#include <SFML/System.hpp>
#include "Core/Grid.h"
#include "Control/InputManager.h"
#include "Interface/GridView.h"
#include "Interface/TestUI.h"
#include <memory>

namespace RedWire
{
	struct Application : sf::RenderWindow
	{
		Application();

		inline const sf::Time& getDeltaTime() { return deltaTime; }
		inline const sf::Time& getTotalTime() { return totalTime; }

		void start();

		void dispatchEvents();

		void update();

		std::unique_ptr<Grid> grid;

		InputManager inputManager;
		GridView gridView;
		TestUI testUI;

	private:
		sf::Clock clock;

		sf::Time deltaTime;
		sf::Time totalTime;
	};
}