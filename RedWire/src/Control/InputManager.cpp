#include <algorithm>
#include <memory>
#include <math.h>
#include <array>
#include "../Application.h"
#include "InputManager.h"
#include "PanTool.h"
#include "RemoveTool.h"
#include "WireAdder.h"
#include "GateAdder.h"
#include "JoinAdder.h"

using namespace RedWire;
using namespace sf;

InputManager::InputManager(Application& application) : application(application), viewCenter(), viewExtend(20.0f), tools()
{
	tools[0] = std::make_unique<PanTool>(*this);
	tools[1] = std::make_unique<RemoveTool>(*this);
	tools[2] = std::make_unique<WireAdder>(*this);
	tools[3] = std::make_unique<GateAdder>(*this);
	tools[4] = std::make_unique<JoinAdder>(*this);
}

void InputManager::onEventPoll(const Event& event)
{
	switch (event.type)
	{
		case Event::MouseWheelScrolled:
		{
			static const float sensitivity = 1.5f;
			static const float minExtend = 6.0f;
			static const float maxExtend = 84.0f;

			float oldExtend = viewExtend;

			viewExtend -= event.mouseWheelScroll.delta * sensitivity;
			viewExtend = std::clamp(viewExtend, minExtend, maxExtend);

			//Zoom origin based on mouse/cursor
			Float2 mouse = getMousePosition();
			Float2 offset = mouse - viewCenter;

			offset *= viewExtend / oldExtend;
			viewCenter = mouse - offset;

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

	float speed = deltaTime.asSeconds() * movementSpeed;
	viewCenter += getMovement() * speed * viewExtend;

	Vector2u windowSize = application.getSize();
	Int2 size = Int2(windowSize.x, windowSize.y);

	float logWidth = std::logf((float)size.x);
	float logHeight = std::logf((float)size.y);

	float logSize = std::exp((logWidth + logHeight) / 2.0f);
	Float2 extend = size.toType<float>() / logSize * viewExtend;

	application.gridView.setView(viewCenter - extend, viewCenter + extend);

	//Tools
	for (size_t i = 0; i < tools.size(); i++)
	{
		std::unique_ptr<Tool>& tool = tools[i];
		if (tool->activationPredicate()) currentTool = i;
	}

	Float2 position = getMousePosition();
	Int2 cell = position.getFloor().toType<int32_t>();
	bool down = Mouse::isButtonPressed(Mouse::Button::Left);

	tools.at(currentTool)->update(position, cell, down, down != leftMousePressed);
	leftMousePressed = down;
}

Float2 InputManager::getMousePosition()
{
	Vector2i mouse = Mouse::getPosition(application);
	Float2 mousePosition = Float2((float)mouse.x, (float)mouse.y);
	return application.gridView.getPosition(mousePosition);
}
