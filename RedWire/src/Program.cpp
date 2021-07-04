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

Application::Application() : RenderWindow( VideoMode::getDesktopMode(), "Red Wire" ),
	grid(std::make_shared<Grid>()), gridView(GridView::DefaultSize, grid)
{
}

void Application::start()
{
	setVerticalSyncEnabled(true);

	//this will be moved to GridView instead and calculated by telling which grid position (top left) it is, then scaling it automatically to fit the gridView
	sf::View targetView(sf::FloatRect(0, 0, (float) gridView.getSize().x, (float) gridView.getSize().y));

	//not using this because It might not be able to render everything on the right
	//targetView.setViewport( sf::FloatRect( 0, 0, .5f, 1.f ) );

	setView(targetView);
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