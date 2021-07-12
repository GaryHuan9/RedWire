#include "Toolbox.h"
#include "CurrentTool.h"
#include "../Application.h"
#include "../Control/InputManager.h"
#include "../Control/Tool.h"

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

	if (ImGui::BeginCombo("Tool", currentTool.getName()))
	{
		for (auto& pair : manager.tools)
		{
			std::unique_ptr<Tool>& tool = pair.second;
			const bool selected = &currentTool == tool.get();

			if (ImGui::Selectable(tool->getName(), selected)) manager.setCurrentTool(tool.get());
			if (selected) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	currentTool.showUI();
}
