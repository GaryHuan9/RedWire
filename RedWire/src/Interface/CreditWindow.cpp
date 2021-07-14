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

		if (ImGui::CollapsingHeader("Assets"))
		{
			ImGui::Text("RedWire Icon: CXRedix");
			ImGui::Text("Font: Jetbrains Mono");
		}
		if (ImGui::CollapsingHeader("Libraries"))
		{
			ImGui::Text("UI core: ImGUI");
			ImGui::Text("Underlying core: SFML");
		}
	}

	setActive(active);
	ImGui::End();
}
