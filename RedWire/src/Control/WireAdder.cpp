#include "WireAdder.h"
#include "Tool.h"
#include "../Core/Grid.h"
#include "../Core/Cell.h"
#include "../Core/Wire.h"

#include <iostream>
#include "imgui.h"

using namespace RedWire;

WireAdder::WireAdder(InputManager& manager) : LineTool(manager)
{}

bool WireAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::E);
}

void WireAdder::setCell(const Int2& cell)
{
	if (!overrideCell)
	{
		Cell* previous = grid->get(cell);
		if (previous && !dynamic_cast<Wire*>(previous)) return;
	}

	bool addJoin = false;

	if (autoAddJoin && getStartCell() != cell)
	{
		int count = 0;

		for (const Int2& offset : Int2::edges4)
		{
			Cell* target = grid->get(cell + offset);
			if (dynamic_cast<Wire*>(target)) ++count;
		}

		if (count > 2) addJoin = true;
	}

	if (!addJoin)
	{
		grid->addWire(cell);

		switch (mode)
		{
			case Mode::unchanged: break;
			case Mode::unpowered: grid->setSource(cell, false); break;
			case Mode::powered:   grid->setSource(cell, true);  break;
		}
	}
	else grid->addJoin(cell);
}

void WireAdder::showUI()
{
	int* ptr = reinterpret_cast<int*>(&mode);

	static const char* modeNames[] = { "Unchanged", "Unpowered", "Powered" };

	ImGui::SliderInt("Mode", ptr, 0, 2, modeNames[*ptr]);
	ImGui::Checkbox("Override Previous", &overrideCell);
	ImGui::Checkbox("Auto Place Joins", &autoAddJoin);

	LineTool::showUI();
}

void WireAdder::showHelpUI()
{
	ImGui::Text("Use the WireAdder by dragging or clicking on the [Grid] to create a line of [Wires]");
	ImGui::Text("All adjacent [Wires] are connected into a bundle, which powers and unpowers collectively");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Keyboard Shortcut: E");
}
