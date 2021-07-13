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
	const static float percent = 0.2f;

	const ImVec2 corner(origin.x + size.x - margin, origin.y + margin);

	if (resetWindowOrientation)
	{
		float width = size.x * percent;

		ImGui::SetNextWindowPos(ImVec2(corner.x - width, corner.y));
		ImGui::SetNextWindowSize(ImVec2(width, width));

		resetWindowOrientation = false;
	}

	if (!ImGui::Begin("Toolbox"))
	{
		ImGui::End();
		return;
	}

	for (auto& section : sections)
	{
		ImGui::PushID(&section);
		section->show();
		ImGui::PopID();
	}

	ImGui::End();
}