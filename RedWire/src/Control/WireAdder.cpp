#include "InputManager.h"
#include "WireAdder.h"
#include "Tool.h"
#include "LineTool.h"
#include "../Type2.h"
#include "../Core/Grid.h"

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
	grid->addWire(cell);

	switch (mode)
	{
		case Mode::unchanged: break;
		case Mode::unpowered: grid->setSource(cell, false); break;
		case Mode::powered:   grid->setSource(cell, true);  break;
	}
}

void WireAdder::showUI()
{
	static const char* modeNames[] = { "Unchanged", "Unpowered", "Powered" };
	ImGui::SliderInt("Mode", &mode, 0, 2, modeNames[mode]);

	LineTool::showUI();
}
