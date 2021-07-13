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
	else if (down) setCell(cell);
}

bool NoteAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Num3);
}

void NoteAdder::setCell(const Int2& cell)
{
	if (grid->get(cell) == nullptr)
		grid->addNote(cell);
}

void NoteAdder::showUI()
{
	ImGui::Checkbox("Draw Lines", &drawLines);
	LineTool::showUI();
}

void NoteAdder::showHelpUI()
{
	ImGui::Text("The NoteAdder adds [Note] cells onto the circuit, which are fully decorative cells that serve no actual functionalities");
	ImGui::Text("To place down [Note] cells, either simply drag to draw freely, or hold down the Shift key to draw in straight lines");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Keyboard Shortcut: Number Key 3");
}
