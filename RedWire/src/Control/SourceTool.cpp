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

void SourceTool::doHelpUI()
{
	ImGui::Text("This controls the source toggling on wire cells, you can toggle on or off a single bundle of wire cell(s)\n\
To use it, click on any wire cell to toggle their powered state");
}
