#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "Control/InputManager.h"
#include "Interface/GridView.h"
#include "Interface/TestUI.h"
#include "Core/Grid.h"

#include <iostream>
#include <memory>

#include <imgui.h>
#include <imgui-SFML.h>

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

	ImGui::SFML::Shutdown();

	return 0;
}

Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire", Style::Default, ContextSettings{ 0, 0, 2, 1, 1, ContextSettings::Attribute::Default, true }),
grid(std::make_unique<Grid>()), inputManager(*this), gridView(*this), testUI(*this), clock()
{

}

void Application::start()
{
	setVerticalSyncEnabled(true);
	ImGui::SFML::Init(*this);
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

		ImGui::SFML::ProcessEvent(event);
		inputManager.onEventPoll(event);
	}
}

void Application::update()
{
	Time lastTime = totalTime;

	totalTime = clock.getElapsedTime();
	deltaTime = totalTime - lastTime;

	/*if (Keyboard::isKeyPressed(Keyboard::Space))*/ grid->update();

	inputManager.update();
	gridView.update();
	testUI.update();

	ImGui::SFML::Update(*this, deltaTime);

	ImGui::Begin("Sample window"); // begin window

	float color[3] = { 0.f, 0.f, 0.f };

	// Background color edit
	if (ImGui::ColorEdit3("Background color", color))
	{

	}

	char windowTitle[255] = "ImGui + SFML = <3";

	// Window title text edit
	ImGui::InputText("Window title", windowTitle, 255);

	if (ImGui::Button("Update window title"))
	{
		// this code gets if user clicks on the button
		// yes, you could have written if(ImGui::InputText(...))
		// but I do this to show how buttons work :)
		//.setTitle(windowTitle);
	}
	ImGui::End(); // end window

	//window.clear(bgColor); // fill background with color
	ImGui::SFML::Render(*this);
}