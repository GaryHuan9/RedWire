#include <algorithm>
#include <memory>
#include <math.h>
#include "../Application.h"
#include "InputManager.h"

using namespace RedWire;
using namespace sf;

InputManager::InputManager(Application& application) : application(application), viewCenter(), viewExtend(20.0f)
{}

void InputManager::onEventPoll(const Event& event)
{
	switch (event.type)
	{
		case Event::MouseWheelScrolled:
		{
			static const float sensitivity = 1.5f;
			static const float minExtend = 6.0f;
			static const float maxExtend = 84.0f;

			viewExtend += event.mouseWheelScroll.delta * sensitivity;
			viewExtend = std::clamp(viewExtend, minExtend, maxExtend);

			break;
		}
	}
}

Float2 getMovement()
{
	return Float2
	(
		Keyboard::isKeyPressed(Keyboard::A) * -1.f + Keyboard::isKeyPressed(Keyboard::D) * 1.f,
		Keyboard::isKeyPressed(Keyboard::W) * -1.f + Keyboard::isKeyPressed(Keyboard::S) * 1.f
	).normalize();
}

void InputManager::update(const Time& deltaTime)
{
	if (!application.hasFocus()) return;

	//View movement
	static const float movementSpeed = 1.5f;
	GridView& gridView = application.gridView;

	float speed = deltaTime.asSeconds() * movementSpeed;
	viewCenter += getMovement() * speed * viewExtend;

	Vector2u windowSize = application.getSize();
	Int2 size = Int2(windowSize.x, windowSize.y);

	float logWidth = std::logf(size.x);
	float logHeight = std::logf(size.y);

	float logSize = std::exp((logWidth + logHeight) / 2.0f);
	Float2 extend = size.toType<float>() / logSize * viewExtend;

	gridView.setView(viewCenter - extend, viewCenter + extend);

	//Placement
	Vector2i mousePoint = Mouse::getPosition(application);
	Float2 mousePosition = Float2(mousePoint.x, mousePoint.y);

	Float2 position = gridView.getPosition(mousePosition);
	Int2 cell = position.getFloor().toType<int32_t>();

	if (Mouse::isButtonPressed(Mouse::Button::Left)) application.grid.addWire(cell);
}
