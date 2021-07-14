#include "HotkeyAction.h"

using namespace RedWire;

bool HotkeyAction::checkActivation()
{
	return activationPredicate() ?
		(initialActivate ? false : initialActivate = true) :
		initialActivate = false;
}