#include "Toolbox.h"
#include "General.h"

#include "imgui.h"

using namespace RedWire;

General::General(Toolbox& toolbox) : Section(toolbox)
{}

void General::show()
{
	if (!ImGui::CollapsingHeader("General")) return;
}
