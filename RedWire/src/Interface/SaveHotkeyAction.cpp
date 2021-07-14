#include "SaveHotkeyAction.h"

#include "../Application.h"
#include "../Control/UIManager.h"
#include "HelpWindow.h"

using namespace RedWire;

SaveHotkeyAction::SaveHotkeyAction(Application& application) : application(application)
{}

void SaveHotkeyAction::doAction()
{
	//we need to seperate this into another class that manages the names of the files n stuff
}

bool SaveHotkeyAction::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}
