#pragma once

#include "Core/Grid.h"
#include "Core/GridView.h"

namespace RedWire
{
	using namespace sf;

	struct Application : RenderWindow
	{
		Application();

		void start();

		void dispatchEvents();

		void update(const sf::Time& deltaTime);

	private:
		//not sure if I should put this here
		std::shared_ptr<Grid> grid;
		GridView gridView;
	};
}