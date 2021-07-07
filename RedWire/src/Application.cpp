#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Application.h"

#include <iostream>

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

// working grid view
/*Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire", Style::Default, ContextSettings{ 0, 0, 2, 1, 1, ContextSettings::Attribute::Default, true }),
clock(), grid(std::make_shared<Grid>()), gridView(GridView::DefaultSize, grid), testUI()
{

}*/

//new grid view
Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire", Style::Default, ContextSettings{ 0, 0, 2, 1, 1, ContextSettings::Attribute::Default, true }),
clock(), grid(std::make_shared<Grid>()), inputManager(std::make_shared<InputManager>()), gridViewNew(grid, Float2(), Float2(30.f, 20.f)), testUI()
{

}

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
		inputManager->onEventPoll(event);
		//gridView.onAppEventPoll(event, *this);
	}
}

void Application::update()
{
	const Time& deltaTime = clock.restart();

	//I seperated this because it's easier to control and easier to understand
	//setView(gridView.getCameraView());

	//gridView.update(*this, deltaTime);

	gridViewNew.update(*this, *inputManager, deltaTime);

	//set the view back to what it should be
	setView(getDefaultView());

	testUI.update(*grid, *this);
}