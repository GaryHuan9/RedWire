#include "Toolbox.h"
#include "Serialization.h"

#include "imgui.h"

using namespace RedWire;

Serialization::Serialization(Toolbox& toolbox) : Section(toolbox)
{}

void Serialization::show()
{
	if (!ImGui::CollapsingHeader("Serialization")) return;
}
