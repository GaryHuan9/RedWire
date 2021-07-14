#include "SaveHotkeyAction.h"

#include "../Application.h"
#include "../Control/UIManager.h"
#include "HelpWindow.h"

using namespace RedWire;

SaveHotkeyAction::SaveHotkeyAction(Application& application) : application(application)
{}

void SaveHotkeyAction::doAction()
{
	//we need a way to access Serialization.cpp or another class that stores the fileName from Serialization since it's a UI
}

bool SaveHotkeyAction::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S);
}
