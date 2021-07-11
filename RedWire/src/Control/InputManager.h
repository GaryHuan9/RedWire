#pragma once

#include <SFML/Window.hpp>
#include <memory>
#include <string>
#include <array>
#include "Tool.h"
#include "../Component.h"
#include "../Type2.h"

namespace RedWire
{
	struct Application;

	struct InputManager : Component
	{
		InputManager(Application& application);

		void onEventPoll(const sf::Event& event);

		static bool isPressed(const sf::Keyboard::Key& key);
		static bool isPressed(const sf::Mouse::Button& button);

		virtual void update() override;

		Tool* getCurrentTool();

		Float2 viewCenter;
		float  viewExtend;

		std::array<std::unique_ptr<Tool>, 8> tools;
		size_t currentTool{ 0 };

	private:

		Float2 getMousePosition(); //Returns the world position of the mouse 
		bool leftMousePressed{};
	};
}

