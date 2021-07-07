#include "InputManager.h"

using namespace RedWire;

InputManager::InputManager()
{

}

void InputManager::onEventPoll(const sf::Event& appEvent)
{
	
}

Float2 InputManager::getMovementDeltaRaw() const
{
	return Float2(
		sf::Keyboard::isKeyPressed(sf::Keyboard::A) * -1.f + sf::Keyboard::isKeyPressed(sf::Keyboard::D) * 1.f,
		sf::Keyboard::isKeyPressed(sf::Keyboard::W) * -1.f + sf::Keyboard::isKeyPressed(sf::Keyboard::S) * 1.f
	);
}
