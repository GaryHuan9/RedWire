#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Program.h"

using namespace RedWire;

int main()
{
	Application application{};

	application.start();

	sf::Clock deltaClock;

	while (application.isOpen())
	{
		application.dispatchEvents();

		application.clear();
		application.update(deltaClock.restart());
		application.display();
	}

	return 0;
}

Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire"),
	grid(std::make_shared<Grid>()), gridView(GridView::DefaultSize, grid)
{
}

void Application::start()
{
	setVerticalSyncEnabled(true);

	setView(gridView.getView());
}

void Application::dispatchEvents()
{
	Event event;

	while (pollEvent(event))
	{
		if (event.type == Event::Closed) close();
	}
}

void Application::update(const sf::Time& deltaTime)
{
	gridView.update(*this);
	gridView.updateTexture();
	gridView.display(*this);
}