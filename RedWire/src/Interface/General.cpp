#include "Toolbox.h"
#include "General.h"
#include "../Application.h"
#include "../Control/InputManager.h"
#include "../Core/TickManager.h"
#include "../Core/Grid.h"
#include "../Control/UIManager.h"
#include "HelpWindow.h"

#include <math.h>
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

	ImGui::SameLine();
	
	if (ImGui::Button("Help"))
	{
		UIManager& uiManager = toolbox.application.find<UIManager>();
		uiManager.find<HelpWindow>().toggleActive();
	}

	showTickControls();

}

void General::showTickControls()
{
	TickManager& manager = toolbox.application.find<TickManager>();
	static const char* names[] = { "Per Second", "Per Frame", "Manual" };

	int* modePtr = reinterpret_cast<int*>(&manager.mode);
	int* ratePtr = reinterpret_cast<int*>(&manager.magnitude);

	ImGui::SliderInt("Tick Mode", modePtr, 0, 2, names[*modePtr]);

	if (ImGui::InputInt("Tick Rate", ratePtr)) manager.magnitude = std::max(1ull, manager.magnitude);
	if (manager.mode == TickManager::Mode::manual && ImGui::Button("Tick")) manager.updateGrid();
}
