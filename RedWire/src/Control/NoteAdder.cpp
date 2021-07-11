#include "NoteAdder.h"
#include "InputManager.h"
#include "../Core/Grid.h"
#include "imgui.h"

using namespace RedWire;

NoteAdder::NoteAdder(InputManager& manager) : LineTool(manager) {}

void NoteAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (pressed != InputManager::isPressed(sf::Keyboard::LShift))
	{
		pressed = !pressed;
		drawLines = !drawLines;
	}

	if (drawLines) doLineDraw(down, cell);
	else if (down) grid->addNote(cell);
}

bool NoteAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Num4);
}

void NoteAdder::setLineCell(const Int2& cell)
{
	grid->addNote(cell);
}

void NoteAdder::showUI()
{
	ImGui::Checkbox("Draw Lines", &drawLines);
	LineTool::showUI();
}
