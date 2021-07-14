#include "CreditWindow.h"

#include "imgui.h"

using namespace RedWire;

CreditWindow::CreditWindow(Application& application) : UIWindow(application)
{}

void CreditWindow::showUI()
{
	bool active = isActive();

	if (ImGui::Begin("Credits", &active, ImGuiWindowFlags_AlwaysHorizontalScrollbar))
	{
		ImGui::Text("RedWire idea: MMXX3VII");
		ImGui::Text("RedWire developers: MMXX3VII & CXRedix");

		ImGui::Dummy(ImVec2(0.f, 10.f));

		ImGui::Text("Used Libraries:");
		ImGui::Text("UI: ImGUI");
		ImGui::Text("Underlying core: SFML");
	}

	setActive(active);
	ImGui::End();
}
