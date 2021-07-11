#include "Toolbox.h"
#include "CurrentTool.h"
#include "../Application.h"
#include "../Control/InputManager.h"

#include "imgui.h"
#include <cstring>
#include <memory>

using namespace RedWire;

CurrentTool::CurrentTool(Toolbox& toolbox) : Section(toolbox)
{}



void CurrentTool::show()
{
	if (!ImGui::CollapsingHeader("Tools", ImGuiTreeNodeFlags_DefaultOpen)) return;

	auto& manager = toolbox.application.find<InputManager>();
	auto& currentTool = *manager.getCurrentTool();

	if (ImGui::BeginCombo("", currentTool.getName(), ImGuiComboFlags_NoArrowButton))
	{
		for (size_t i = 0; i < manager.tools.size(); i++)
		{
			std::unique_ptr<Tool>& tool = manager.tools[i];
			const bool selected = i == manager.currentTool;

			if (ImGui::Selectable(tool->getName(), selected)) manager.currentTool = i;

			if (selected) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::SameLine(0.0f, 8.0f);
	ImGui::Text("Current Tool");

	currentTool.showUI();
}
