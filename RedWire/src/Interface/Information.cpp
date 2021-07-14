#include "Toolbox.h"
#include "Information.h"

#include "../Application.h"
#include "../Core/Grid.h"

#include "imgui.h"

using namespace RedWire;

Information::Information(Toolbox& toolbox) : Section(toolbox)
{}

void Information::show()
{
	if (!ImGui::CollapsingHeader("Information")) return;

	// == calculate FPS ==

	if (currentInterval >= FPS_UPDATE_INTERVAL)
	{
		lastAverageFPS = (float)frameCount / currentInterval;

		frameCount = 0u;
		currentInterval = 0.f;
	}

	frameCount++;

	auto& application = toolbox.application;

	currentInterval += application.getDeltaTime().asSeconds();

	// == draw text ==
	const std::string tileCountString = std::string("Tile Count: ") + std::to_string(application.grid->getTileCount());
	const std::string wireCountString = std::string("Wire Count: ") + std::to_string(application.grid->getWireCount());
	const std::string gateCountString = std::string("Gate Count: ") + std::to_string(application.grid->getGateCount());
	const std::string FPSString = std::string("FPS: ") + std::to_string(lastAverageFPS);

	ImGui::Text(tileCountString.c_str());
	ImGui::Text(wireCountString.c_str());
	ImGui::Text(gateCountString.c_str());
	ImGui::Text(FPSString.c_str());
}
