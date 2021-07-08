#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
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

		void update(const sf::Time& deltaTime);

		Application& application;

		Float2 viewCenter;
		float  viewExtend;

	private:

		std::array<std::unique_ptr<Tool>, 5> tools;

		size_t currentTool{ 0 };
		bool leftMousePressed{};
	};
}

