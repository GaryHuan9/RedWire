#include "Toolbox.h"
#include "Information.h"
#include "HelpWindow.h"
#include "CreditWindow.h"

#include "../Application.h"
#include "../Core/Grid.h"
#include "../Control/UIManager.h"

#include "imgui.h"

using namespace RedWire;

Information::Information(Toolbox& toolbox) : Section(toolbox)
{}

void Information::show()
{
	if (!ImGui::CollapsingHeader("Information")) return;

	// == calculate FPS ==

	if (currentInterval >= FPSUpdateInterval)
	{
		lastAverageFPS = (float)frameCount / currentInterval;

		frameCount = 0u;
		currentInterval = 0.f;
	}

	frameCount++;

	auto& application = toolbox.application;

	currentInterval += application.getDeltaTime().asSeconds();

	// == draw ==

	if (ImGui::Button("Help"))
	{
		UIManager& uiManager = toolbox.application.find<UIManager>();
		uiManager.find<HelpWindow>().toggleActive();
	}

	ImGui::SameLine();

	if (ImGui::Button("Credits"))
	{
		UIManager& uiManager = toolbox.application.find<UIManager>();
		uiManager.find<CreditWindow>().toggleActive();
	}

	ImGui::Text("Tile Count: %u", application.grid->getTileCount());
	ImGui::Text("Wire Count: %u", application.grid->getWireCount());
	ImGui::Text("Inverter Count: %u", application.grid->getInverterCount());
	ImGui::Text("Transistor Count: %u", application.grid->getTransistorCount());
	ImGui::Text("FPS: %0.2f", lastAverageFPS);
}
