#include <algorithm>
#include <string>
#include <memory>
#include <math.h>
#include <array>
#include "../Application.h"
#include "../Component.h"
#include "../Interface/GridView.h"
#include "InputManager.h"
#include "HandTool.h"
#include "RemoveTool.h"
#include "WireAdder.h"
#include "GateAdder.h"
#include "JoinAdder.h"
#include "NoteAdder.h"
#include "Clipboard.h"
#include "AreaSerializer.h"

#include "imgui.h"

using namespace RedWire;
using namespace sf;

InputManager::InputManager(Application& application) : Component(application), viewCenter(), viewExtend(20.0f), tools()
{
	tools[0] = std::make_unique<HandTool>(*this);
	tools[1] = std::make_unique<RemoveTool>(*this);
	tools[2] = std::make_unique<WireAdder>(*this);
	tools[3] = std::make_unique<GateAdder>(*this);
	tools[4] = std::make_unique<JoinAdder>(*this);
	tools[5] = std::make_unique<Clipboard>(*this);
	tools[6] = std::make_unique<AreaSerializer>(*this);
	tools[7] = std::make_unique<NoteAdder>(*this);
}

bool imGuiStoleInput()
{
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse) return true;
	if (io.WantCaptureKeyboard) return true;

	return false;
}

void InputManager::onEventPoll(const Event& event)
{
	if (imGuiStoleInput()) return;

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

bool InputManager::isPressed(const Keyboard::Key& key)
{
	if (imGuiStoleInput()) return false;
	return Keyboard::isKeyPressed(key);
}

bool InputManager::isPressed(const Mouse::Button& button)
{
	if (imGuiStoleInput()) return false;
	return Mouse::isButtonPressed(button);
}

Float2 getMovement()
{
	return Float2
	(
		InputManager::isPressed(Keyboard::A) * -1.f + InputManager::isPressed(Keyboard::D) * 1.f,
		InputManager::isPressed(Keyboard::W) * -1.f + InputManager::isPressed(Keyboard::S) * 1.f
	).normalize();
}

void InputManager::update()
{
	//Tools
	Float2 position = getMousePosition();
	Int2 cell = position.getFloor().toType<int32_t>();

	bool left = isPressed(Mouse::Button::Left);
	bool middle = isPressed(Mouse::Button::Middle);

	for (size_t i = 0; i < tools.size(); i++)
	{
		auto& tool = tools[i];

		bool activate = tool->activationPredicate();
		if (!activate || currentTool == i) continue;

		setCurrentTool(i);
	}

	getCurrentTool()->update(position, cell, left, left != leftMouse);

	//Mouse view movement
	if (middle)
	{
		if (!middleMouse) pressedPosition = position;
		else viewCenter += pressedPosition - position;
	}

	leftMouse = left;
	middleMouse = middle;

	//Keyboard view movement
	static const float movementSpeed = 1.5f;
	auto& view = application.find<GridView>();

	float speed = application.getDeltaTime().asSeconds() * movementSpeed;
	viewCenter += getMovement() * speed * viewExtend;

	Vector2u windowSize = application.getSize();
	Int2 size = Int2(windowSize.x, windowSize.y);

	float logWidth = std::logf((float)size.x);
	float logHeight = std::logf((float)size.y);

	float logSize = std::exp((logWidth + logHeight) / 2.0f);
	Float2 extend = size.toType<float>() / logSize * viewExtend;

	view.setView(viewCenter - extend, viewCenter + extend);
}

void InputManager::setCurrentTool(size_t tool)
{
	if (currentTool == tool) return;
	auto& disabling = *getCurrentTool();

	disabling.onDisable();
	currentTool = tool;
}

Tool* InputManager::getCurrentTool() const
{
	return tools.at(currentTool).get();
}

Float2 InputManager::getMousePosition()
{
	Vector2i mouse = Mouse::getPosition(application);
	Float2 mousePosition = Float2((float)mouse.x, (float)mouse.y);
	return application.find<GridView>().getPosition(mousePosition);
}
