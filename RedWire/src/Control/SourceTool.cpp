#include "SourceTool.h"
#include "InputManager.h"
#include "../Application.h"
#include "../Core/Grid.h"
#include "../Core/Wire.h"

#include "imgui.h"

using namespace RedWire;

SourceTool::SourceTool(InputManager& manager) : Tool(manager)
{}

void SourceTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	Wire* wire = dynamic_cast<Wire*>(grid->get(cell));
	if (!changed || !down || wire == nullptr) return;

	bool* pointer = nullptr;

	switch (type)
	{
		case Type::permanent: pointer = &wire->isSource;    break;
		case Type::temporary: pointer = &wire->poweredLast; break;
	}

	if (pointer == nullptr) return;

	switch (mode)
	{
		case Mode::toggle:  *pointer = !*pointer; break;
		case Mode::power:   *pointer = true;      break;
		case Mode::unpower: *pointer = false;     break;
	}
}

bool SourceTool::activationPredicate()
{
	return InputManager::isPressed(sf::Mouse::Right);
}

void SourceTool::showUI()
{
	int* modePtr = reinterpret_cast<int*>(&mode);
	int* typePtr = reinterpret_cast<int*>(&type);

	static const char* modeNames[] = { "Toggle", "Power", "Unpower" };
	static const char* typeNames[] = { "Permanent", "Temporary" };

	ImGui::SliderInt("Mode", modePtr, 0, 2, modeNames[*modePtr]);
	ImGui::SliderInt("Type", typePtr, 0, 1, typeNames[*typePtr]);
}

void SourceTool::showHelpUI()
{
	ImGui::Text("The SourceTool is the default tool; it is used to control whether a [Wire] is a power source");
	ImGui::Text("Simply click on a [Wire] will change the power state of that [Wire] from Off to On or vice versa");
	ImGui::Text("Pressing the Right Mouse Button will switch from any selected tool back to the default SourceTool");
}
