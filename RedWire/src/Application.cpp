#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "Control/InputManager.h"
#include "Interface/GridView.h"
#include "Interface/TestUI.h"
#include "Core/Grid.h"

#include <iostream>

using namespace RedWire;
using namespace sf;

int main()
{
	Application application;

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

Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire", Style::Default, ContextSettings{ 0, 0, 2, 1, 1, ContextSettings::Attribute::Default, true }),
grid(), inputManager(*this), gridView(*this), testUI(*this), clock()
{

}

void Application::start()
{
	setVerticalSyncEnabled(true);
	clock.restart();
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
				Vector2f size((float)event.size.width, (float)event.size.height);
				setView(View(size / 2.0f, size));

				break;
			}
		}

		inputManager.onEventPoll(event);
	}
}

void Application::update()
{
	Time lastTime = totalTime;

	totalTime = clock.getElapsedTime();
	deltaTime = totalTime - lastTime;

	inputManager.update();
	gridView.update();
	testUI.update(); //yes >:)
}