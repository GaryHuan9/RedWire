#include "imgui.h"

#include "HelpWindow.h"
#include "../../Application.h"
#include "../../Control/InputManager.h"
#include "../../Control/Tool.h"

#include <string>

using namespace RedWire;

HelpWindow::HelpWindow(Application& application) : UIWindow(application)
{}

void HelpWindow::showUI()
{
	bool active = isActive();

	if (ImGui::Begin("Help", &active, ImGuiWindowFlags_HorizontalScrollbar))
	{
		auto& inputManager = application.find<InputManager>();

		ImGui::Text("RedWire is a 2D logic circuit builder made by MMX3VII and CXRedix");

		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		ImGui::Text("The [Toolbox] consists of multiple sections that can be used to control and monitor all nessessary parameters and information");
		ImGui::Text("To start making a circuit, place down [Wires] by selecting the [WireAdder] in the [Toolbox] and start drawing in the square [Grid]");

		if (ImGui::CollapsingHeader("How to basics"))
		{
			ImGui::Text("You can navigate around the [Grid] by dragging with the [Middle Mouse Button] or pressing the WASD keys");
			ImGui::Text("Note that all navigation and keyboard shortcuts are disabled while you are interacting with the [Toolbox]");

			ImGui::Text("The circuit logic is simulated based on [Ticks], which are usually triggered automatically as time passes");
			ImGui::Text("You can control the [Ticks] in the [General] section of [Toolbox]");

			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::Text("More information about individual tools can be found below in the Tools section");
		}

		if (ImGui::CollapsingHeader("Tools"))
		{
			if (ImGui::BeginTabBar("Tools Tab"))
			{
				for (auto& item : inputManager.tools)
				{
					if (!ImGui::BeginTabItem(item.second->getName())) continue;

					ImGui::PushID(&item);
					item.second->showHelpUI();
					ImGui::PopID();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
	}

	setActive(active);
	ImGui::End();
}
