#include "Toolbox.h"
#include "CurrentTool.h"
#include "../Application.h"
#include "../Control/InputManager.h"

#include "imgui.h"

using namespace RedWire;

CurrentTool::CurrentTool(Toolbox& toolbox) : Section(toolbox)
{}

void CurrentTool::show()
{
	if (!ImGui::CollapsingHeader("CurrentTool", ImGuiTreeNodeFlags_DefaultOpen)) return;

	//Temporary
	ImGui::Text(toolbox.application.find<InputManager>().getCurrentToolLabel().c_str());
}
