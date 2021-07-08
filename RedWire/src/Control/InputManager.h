#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <array>
#include "../Type2.h"
#include "Tool.h"

namespace RedWire
{
	struct Application;

	struct InputManager
	{
		InputManager(Application& application);

		void onEventPoll(const sf::Event& event);

		void update();

		std::string getCurrentToolLabel();

		Application& application;

		Float2 viewCenter;
		float  viewExtend;

	private:

		Float2 getMousePosition(); //Returns the world position of the mouse 

		std::array<std::unique_ptr<Tool>, 6> tools;

		size_t currentTool{ 0 };
		bool leftMousePressed{};
	};
}

