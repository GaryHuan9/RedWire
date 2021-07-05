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

		void update();

	private:

		std::shared_ptr<Grid> grid;

		Clock clock;
		GridView gridView;
	};
}