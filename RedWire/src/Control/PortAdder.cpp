#include "PortAdder.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

#include "imgui.h"

using namespace RedWire;

PortAdder::PortAdder(InputManager& manager) : Tool(manager)
{}

void PortAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (InputManager::isPressed(sf::Keyboard::Num1)) mode = Mode::gate;
	if (InputManager::isPressed(sf::Keyboard::Num2)) mode = Mode::join;

	if (down && changed)
	{
		switch (mode)
		{
			case Mode::gate: grid->addGate(cell); break;
			case Mode::join: grid->addJoin(cell); break;
		}
	}
}

bool PortAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Num1)
		|| InputManager::isPressed(sf::Keyboard::Num2);
}

void PortAdder::showUI()
{
	static const char* modeNames[] = { "Gate", "Join" };
	ImGui::SliderInt("Mode", &mode, 0, 1, modeNames[mode]);
}