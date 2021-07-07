#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "Core/Grid.h"
#include "Core/GridView.h"
#include "Core/InputManager.h"
#include "Core/TestUI.h"

#include <iostream>

using namespace RedWire;
using namespace sf;

int main()
{
	Application application;

	application.start();

	sf::Clock deltaClock;

	while (application.isOpen())
	{
		application.dispatchEvents();

		application.clear();
		application.update();
		application.display();
	}

	return 0;
}

Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire", Style::Default, ContextSettings{ 0, 0, 2, 1, 1, ContextSettings::Attribute::Default, true }),
grid(), inputManager(*this), gridView(*this), testUI(/* allow this -> *this */), clock()
{

}

void Application::start()
{
	setVerticalSyncEnabled(true);
	grid.addWire(Int2(3, 4));
	grid.addWire(Int2(3, 5));
	grid.addWire(Int2(3, 6));
}

void Application::dispatchEvents()
{
	Event event;

	while (pollEvent(event))
	{
		switch (event.type)
		{
			case Event::Closed:
			{
				close();
				break;
			}
			case Event::Resized:
			{
				Vector2f size(event.size.width, event.size.height);
				setView(View(size / 2.0f, size));

				break;
			}
		}

		inputManager.onEventPoll(event);
	}
}

void Application::update()
{
	const Time& deltaTime = clock.restart();

	inputManager.update(deltaTime);
	gridView.update();
	testUI.update(grid, *this, deltaTime); //Not gonna touch this but cant we send the grid when we init/construct testUI?
}