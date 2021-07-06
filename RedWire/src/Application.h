#pragma once

#include "Core/Grid.h"
#include "Core/GridView.h"
#include "Core/TestUI.h"

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

		Clock clock;

		std::shared_ptr<Grid> grid;
		GridView gridView;

		// == UI ==
		TestUI testUI;
	};
}