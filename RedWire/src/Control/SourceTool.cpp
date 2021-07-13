#include "SourceTool.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Application.h"
#include "../Type2.h"
#include "../Core/Grid.h"
#include "imgui.h"
#include <SFML/System.hpp>

using namespace RedWire;

SourceTool::SourceTool(InputManager& manager) : Tool(manager)
{}

void SourceTool::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!changed) return;

	//Check toggle source
	static const sf::Time maxDelay = sf::seconds(0.3f);
	sf::Time time = manager.application.getTotalTime();

	if (down) pressedTime = time;
	else if (time - pressedTime < maxDelay)
	{
		grid->setSource(cell, !grid->getSource(cell));
	}
}

bool SourceTool::activationPredicate()
{
	return InputManager::isPressed(sf::Mouse::Right);
}

void SourceTool::showHelpUI()
{
	ImGui::Text("The SourceTool is the default tool; it is used to control whether a [Wire] is a power source");
	ImGui::Text("Simply click on a [Wire] will change the power state of that [Wire] from Off to On or vice versa");
	ImGui::Text("Pressing the Right Mouse Button will switch from any selected tool back to the default SourceTool");
}
