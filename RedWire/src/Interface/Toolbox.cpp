#include "Toolbox.h"
#include "imgui.h"

using namespace RedWire;

Toolbox::Toolbox(Application& application) : application(application)
{}

void Toolbox::update()
{
	ImGui::Begin("Toolbox");

	ImGui::End();
}
