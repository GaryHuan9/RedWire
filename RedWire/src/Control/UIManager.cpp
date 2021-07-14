#include "imgui.h"

#include "UIManager.h"
#include "../Interface/HelpWindow.h"
#include "../Interface/Toolbox.h"

using namespace RedWire;


UIManager::UIManager(Application& application) : Component(application), windows()
{
#define make_window(type) windows[typeid(type)] = std::make_unique<type>(application)

	make_window(HelpWindow);
	make_window(Toolbox);

#undef make_window // undefine macro make_window to make it private
}


void UIManager::update()
{
	for (auto& window : windows)
	{
		ImGui::PushID(&window);
		window.second->update();
		ImGui::PopID();
	}
}
