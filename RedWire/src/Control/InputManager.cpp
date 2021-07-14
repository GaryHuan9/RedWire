#include <algorithm>
#include <math.h>
#include "../Application.h"
#include "../Interface/GridView.h"
#include "InputManager.h"
#include "SourceTool.h"
#include "RemoveTool.h"
#include "WireAdder.h"
#include "PortAdder.h"
#include "NoteAdder.h"
#include "Clipboard.h"

#include "../Interface/SaveHotkeyAction.h"

#include "imgui.h"

using namespace RedWire;
using namespace sf;

InputManager::InputManager(Application& application) : Component(application), viewCenter(), viewExtend(20.0f), tools()
{
#define make_tool(type) tools[typeid(type)] = std::make_unique<type>(*this)

	make_tool(SourceTool);
	make_tool(RemoveTool);
	make_tool(WireAdder);
	make_tool(PortAdder);
	make_tool(Clipboard);
	make_tool(NoteAdder);

#undef make_tool

#define make_hotkey(type) hotkeyActions.push_back(std::make_unique<type>(application))

	make_hotkey(SaveHotkeyAction);

#undef make_hotkey

	currentTool = tools[typeid(SourceTool)].get();
}

bool imGuiStoleInput()
{
	ImGuiIO& io = ImGui::GetIO();

	//not the best solution but it works
	if (io.WantCaptureMouse && ImGui::IsAnyMouseDown()) return true;
	if (io.WantCaptureKeyboard) return true;

	return false;
}

static const float minExtend = 6.0f;
static const float maxExtend = 84.0f;

void InputManager::onEventPoll(const Event& event)
{
	if (imGuiStoleInput()) return;

	switch (event.type)
	{
		case Event::MouseWheelScrolled:
		{
			static const float sensitivity = 1.8f;

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

	for (auto& pair : tools)
	{
		Tool* tool = pair.second.get();

		bool activate = tool->activationPredicate();
		if (!activate || currentTool == tool) continue;

		setCurrentTool(tool);
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
	viewExtend = std::clamp(viewExtend, minExtend, maxExtend);

	Vector2u windowSize = application.getSize();
	Int2 size = Int2(windowSize.x, windowSize.y);

	float logWidth = std::logf((float)size.x);
	float logHeight = std::logf((float)size.y);

	float logSize = std::exp((logWidth + logHeight) / 2.0f);
	Float2 extend = size.toType<float>() / logSize * viewExtend;

	view.setView(viewCenter - extend, viewCenter + extend);

	//Hotkey execution, we don't put this in onEventPoll because if we focus on any ImGUI window, all kinds of hotkeys can't execute
	for (auto& action : hotkeyActions)
	{
		if (!action->checkActivation()) continue;

		action->doAction();
	}
}

void InputManager::setCurrentTool(Tool* tool)
{
	if (currentTool == tool) return;
	if (tools.find(typeid(*tool)) == tools.end()) throw std::exception("Invalid tool");

	currentTool->onDisable();
	currentTool = tool;
}

Tool* InputManager::getCurrentTool() const
{
	return currentTool;
}

Float2 InputManager::getMousePosition()
{
	Vector2i mouse = Mouse::getPosition(application);
	Float2 mousePosition = Float2((float)mouse.x, (float)mouse.y);
	return application.find<GridView>().getPosition(mousePosition);
}
