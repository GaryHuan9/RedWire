#include "../Application.h"
#include "../Component.h"
#include "../Type2.h"
#include "Section.h"
#include "Toolbox.h"
#include "imgui.h"

#include "General.h"
#include "CurrentTool.h"
#include "Serialization.h"
#include "Information.h"

using namespace RedWire;

Toolbox::Toolbox(Application& application) : Component(application), sections()
{
	sections[0] = std::make_unique<General>(*this);
	sections[1] = std::make_unique<CurrentTool>(*this);
	sections[2] = std::make_unique<Serialization>(*this);
	sections[3] = std::make_unique<Information>(*this);
}

void Toolbox::update()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	const ImVec2 origin = viewport->WorkPos;
	const ImVec2 size = viewport->WorkSize;

	const static float margin = 36.0f;
	const static Float2 percent(0.2f, 0.3f);

	const ImVec2 corner(origin.x + size.x - margin, origin.y + margin);

	ImGui::SetNextWindowPos(ImVec2(corner.x - size.x * percent.x, corner.y), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(size.x * percent.x, size.y * percent.x), ImGuiCond_Appearing);

	if (!ImGui::Begin("Toolbox"))
	{
		ImGui::End();
		return;
	}

	for (auto& section : sections)
	{
		section->show();
	}

	ImGui::End();
}