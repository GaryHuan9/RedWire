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

		Tool* getCurrentTool() const;
		void setCurrentTool(size_t tool);

		Float2 viewCenter;
		float  viewExtend;

		std::array<std::unique_ptr<Tool>, 7> tools;

	private:
		Float2 getMousePosition(); //Returns the world position of the mouse

		bool leftMouse{};
		bool middleMouse{};

		size_t currentTool{ 0 };
		Float2 pressedPosition;
	};
}

