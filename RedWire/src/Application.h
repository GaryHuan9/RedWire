#pragma once

#include "Core/Grid.h"
#include "Core/GridView.h"
#include "Core/GridViewNew.h"
#include "Core/InputManager.h"
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
		std::shared_ptr<InputManager> inputManager;
		//GridView gridView;

		//InputManager inputManager;
		GridViewNew gridViewNew;

		// == UI ==
		TestUI testUI;
	};
}