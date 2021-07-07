#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Type2.h"

namespace RedWire
{
	struct Application;

	struct InputManager
	{
		InputManager(Application& application);

		void onEventPoll(const sf::Event& event);

		void update(const sf::Time& deltaTime);

	private:
		Float2 viewCenter;
		float  viewExtend;

		Application& application;
	};
}

