#include "NoteAdder.h"
#include "LineTool.h"
#include "InputManager.h"

#include "../Interface/TextProjector.h"
#include "../Core/Region.h"
#include "../Core/Grid.h"
#include "../Type2.h"

#include <string>

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

using namespace RedWire;

NoteAdder::NoteAdder(InputManager& manager) : LineTool(manager), projector("Assets/Fonts/RedWireNote.txt")
{}

void NoteAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (pressed != InputManager::isPressed(sf::Keyboard::LShift))
	{
		pressed = !pressed;
		drawLines = !drawLines;
	}

	if (drawLines) LineTool::update(position, cell, down, changed);
	else if (down) setCell(cell);
}

bool NoteAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Z);
}

void NoteAdder::setCell(const Int2& cell)
{
	if (grid->get(cell) == nullptr) grid->addNote(cell);
}

void NoteAdder::showUI()
{
	ImGui::Checkbox("Draw Lines", &drawLines);
	ImGui::InputText("Text", &inputText);

	bool disabled = inputText.length() == 0;

	if (disabled)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * .5f);
	}

	if (ImGui::Button("Confirm"))
	{
		Region region = projector.fromText(inputText);
		region.pasteTo(Int2(0, 0), *grid);
	}

	if (disabled)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	LineTool::showUI();
}

void NoteAdder::showHelpUI()
{
	ImGui::Text("The NoteAdder adds [Note] cells onto the circuit, which are fully decorative cells that serve no actual functionalities");
	ImGui::Text("To place down [Note] cells, either simply drag to draw freely, or hold down the Shift key to draw in straight lines");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Keyboard Shortcut: Number Key Z");
}
