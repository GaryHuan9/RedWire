#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Application.h"
#include "Core/Grid.h"

#include "Control/InputManager.h"
#include "Interface/GridView.h"
#include "Interface/Toolbox.h"
#include "Core/TickManager.h"
#include "Control/UIManager.h"

#include <iostream>
#include <memory>

#include "imgui.h"
#include "imgui-SFML.h"

using namespace RedWire;
using namespace sf;

int main()
{
	Application application;

	application.start();

	while (application.isOpen())
	{
		application.dispatchEvents();

		if (application.hasFocus())
		{
			application.clear();
			application.update();
			application.display();
		}
	}

	ImGui::SFML::Shutdown();

	return 0;
}

Application::Application() : RenderWindow(VideoMode::getDesktopMode(), "Red Wire", Style::Default, ContextSettings{ 0, 0, 2, 1, 1, ContextSettings::Attribute::Default, true }),
grid(std::make_unique<Grid>()), components(), clock()
{
#define make_component(type) components[typeid(type)] = std::make_unique<type>(*this);

	make_component(InputManager);
	make_component(TickManager);
	make_component(GridView);
	make_component(UIManager);

	/*components[typeid(InputManager)] = std::make_unique<InputManager>(*this);
	components[typeid(TickManager)] = std::make_unique<TickManager>(*this);
	components[typeid(GridView)] = std::make_unique<GridView>(*this);
	components[typeid(UIManager)] = std::make_unique<UIManager>(*this);*/

#undef make_component

	sf::Image icon;
	icon.loadFromFile("Assets/Logo/RedWireIcon.png");

	setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Application::start()
{
	setVerticalSyncEnabled(true);
	ImGui::SFML::Init(*this, false);

	clock.restart();

	auto& io = ImGui::GetIO();
	auto& style = ImGui::GetStyle();

	io.IniFilename = nullptr;
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/JetBrainsMono/JetBrainsMono-Bold.ttf", 18.0f);
	io.ConfigFlags |= ImGuiConfigFlags_IsSRGB | ImGuiConfigFlags_NavEnableKeyboard;

	style.FrameBorderSize = 1;
	style.TabRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.ItemInnerSpacing.x = 10.0f;
	style.WindowPadding = ImVec2(10.0f, 5.0f);

	ImGui::SFML::UpdateFontTexture();
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

		if (event.key.code != Keyboard::Unknown && hasFocus())
		{
			ImGui::SFML::ProcessEvent(event);
			find<InputManager>().onEventPoll(event);
		}
	}
}

void Application::update()
{
	const Time lastTime = totalTime;

	totalTime = clock.getElapsedTime();
	deltaTime = totalTime - lastTime;

	ImGui::SFML::Update(*this, deltaTime);

	for (auto& component : components) component.second->update();

	ImGui::SFML::Render(*this);
}