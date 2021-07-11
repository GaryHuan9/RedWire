#include "Toolbox.h"
#include "Information.h"

#include "imgui.h"

using namespace RedWire;

Information::Information(Toolbox& toolbox) : Section(toolbox)
{}

void Information::show()
{
	if (!ImGui::CollapsingHeader("Information")) return;
}
