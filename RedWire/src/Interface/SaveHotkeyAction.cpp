#include "SaveHotkeyAction.h"

#include "../Application.h"
#include "../Control/UIManager.h"
#include "../Control/SaveManager.h"
#include "HelpWindow.h"

using namespace RedWire;

SaveHotkeyAction::SaveHotkeyAction(Application& application) : application(application)
{}

void SaveHotkeyAction::doAction()
{
	SaveManager& manager = application.find<SaveManager>();

	manager.saveCircuit();
}

bool SaveHotkeyAction::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}
