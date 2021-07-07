#pragma once
#include <SFML/Graphics.hpp>

#include "Type2.h"

namespace RedWire
{
	class InputManager
	{
	public:
		InputManager();
		void onEventPoll(const sf::Event& appEvent);

		//getters
		Float2 getMovementDeltaRaw() const;
	};
}

