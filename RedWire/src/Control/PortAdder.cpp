#include "PortAdder.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Core/Grid.h"

#include "imgui.h"

using namespace RedWire;

PortAdder::PortAdder(InputManager& manager) : Tool(manager)
{}

void PortAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (InputManager::isPressed(sf::Keyboard::Num1)) mode = Mode::gate;
	if (InputManager::isPressed(sf::Keyboard::Num2)) mode = Mode::bridge;

	if (down && changed && (overrideCell || grid->get(cell) == nullptr))
	{
		switch (mode)
		{
			case Mode::gate: grid->addGate(cell); break;
			case Mode::bridge: grid->addBridge(cell); break;
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
	int* ptr = reinterpret_cast<int*>(&mode);

	static const char* modeNames[] = { "Gate", "Bridge" };
	ImGui::SliderInt("Mode", ptr, 0, 1, modeNames[*ptr]);
	ImGui::Checkbox("Override Previous", &overrideCell);
}

void PortAdder::showHelpUI()
{
	ImGui::Text("The PortAdder can add two different types of [Port] ([Gate] and [Bridge]) by simply clicking on the [Grid]");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("The [Gate] only functions when three [Wires] adjacent to it form a 'T' shape:");
	ImGui::Text("The orientation of a [Gate] is determined by the side that is missing a [Wire]");
	ImGui::Text("The [Wire] that is present on the opposite side of the missing side is the Control");
	ImGui::Text("To differentiate the two other wires, look at the [Gate] so the Control points downwards");
	ImGui::Text("When the condition above is fulfilled, the left is the Input and the right is the Output");
	ImGui::Text("If the Control is powered Off, the state of the Input is simply copied to the Output");
	ImGui::Text("If the Control is powered On, the state of the Input is inverted and sent to the Output");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("The [Bridge] serves as a junction that links two [Wires] on the same axis together");
	ImGui::Text("It allows the creation of '+' shaped circuits to transmit signals without interference");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Keyboard Shortcut (Gate): Number Key 1");
	ImGui::Text("Keyboard Shortcut (Bridge): Number Key 2");
}
