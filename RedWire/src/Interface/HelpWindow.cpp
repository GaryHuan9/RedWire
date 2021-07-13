#include "HelpWindow.h"
#include "imgui.h"

using namespace RedWire;

HelpWindow::HelpWindow(Application& application) : UIWindow(application)
{}

void HelpWindow::doUI()
{
	if (ImGui::Begin("Help"))
	{
		ImGui::Text("Help window here...");
		ImGui::End();
	}
}
