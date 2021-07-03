#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Program.h"

using namespace sf;
using namespace RedWire;

int main()
{
	Application application{};

	application.start();

	while (application.isOpen())
	{
		application.dispatchEvents();

		application.clear();
		application.update();
		application.display();
	}

	return 0;
}

namespace RedWire
{
	Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire")
	{

	}

	void Application::start()
	{

	}

	void Application::dispatchEvents()
	{
		Event event;

		while (pollEvent(event))
		{
			if (event.type == Event::Closed) close();
		}
	}

	void Application::update()
	{

	}
};