#include "imgui.h"

#include "UIManager.h"
#include "../Interface/HelpWindow.h"

using namespace RedWire;

UIManager::UIManager(Application& application) : Component(application), windows()
{
	windows[typeid(HelpWindow)] = std::make_unique<HelpWindow>(application);
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
