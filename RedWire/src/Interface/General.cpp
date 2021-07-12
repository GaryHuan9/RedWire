#include "Toolbox.h"
#include "General.h"
#include "../Application.h"
#include "../Control/InputManager.h"
#include "../Core/Grid.h"

#include "imgui.h"

using namespace RedWire;

General::General(Toolbox& toolbox) : Section(toolbox)
{}

void General::show()
{
	if (!ImGui::CollapsingHeader("General")) return;

	if (ImGui::Button("Exit")) toolbox.application.close();

	ImGui::SameLine();

	if (ImGui::Button("Reset Toolbox")) toolbox.resetWindow();

	ImGui::SameLine();

	if (ImGui::Button("Center View"))
	{
		Int2 min;
		Int2 max;

		toolbox.application.grid->findBorder(min, max);
		Float2 extend = (max - min).toType<float>() / 2.0f;

		InputManager& inputManager = toolbox.application.find<InputManager>();
		inputManager.viewCenter = extend + Float2(0.5f) + min.toType<float>();
	}
}
