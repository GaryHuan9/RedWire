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
	grid->setSource(cell, addSource);
}

void WireAdder::showUI()
{
	ImGui::Checkbox("Source", &addSource);
	LineTool::showUI();
}
