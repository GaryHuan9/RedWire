#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Application.h"

using namespace RedWire;

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
clock(), grid(std::make_shared<Grid>()), gridView(GridView::DefaultSize, grid)
{}

void Application::start()
{
	setVerticalSyncEnabled(true);
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
	const Time& deltaTime = clock.restart();

	gridView.update(*this, deltaTime);
}