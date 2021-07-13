#include "imgui.h"

#include "HelpWindow.h"
#include "../Application.h"
#include "../Control/InputManager.h"
#include "../Control/Tool.h"

#include <string>

using namespace RedWire;

HelpWindow::HelpWindow(Application& application) : UIWindow(application)
{}

void HelpWindow::doUI()
{
	if (ImGui::Begin("Help"))
	{
		//NOTE: this is not final! I suck at describing
		InputManager& inputManager = application.find<InputManager>();

		ImGui::Text("RedWire is a 2D logic circuit builder made by MMXXX-VII and CXRedix\n\n\
use [Middle mouse button] to pan around\n\
In order to make a circuit, we place [Cells] using [Tools] inside the [Toolbox] into the grid shown in front of you\n\
The Toolbox consists of multiple [Sections], as you can tell, you are already familiary with it since how come you came here :)\n\n"); // except you are reading the code lol *wink wink*

		if (ImGui::CollapsingHeader("How to basics"))
		{
			ImGui::Text("use [Middle mouse button] to pan around the grid\n\
To select a [Tool], navigate to the [Toolbox] and inside [Tools] section, choose a Tool of choice from the drop down.");
		}

		if (ImGui::CollapsingHeader("Tools"))
		{
			ImGui::Text((std::string("There are currently ") + std::to_string(inputManager.tools.size()) + " tools in redwire, each of them have their unique use case\n\n").c_str());

			if (ImGui::BeginTabBar("Tools Tab"))
			{
				for (auto& item : inputManager.tools)
				{
					if (ImGui::BeginTabItem((std::string(item.second->getName()).c_str())))
					{
						ImGui::PushID(&item);
						item.second->doHelpUI();
						ImGui::PopID();
						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
		}

		ImGui::End();
	}
}
