#include "NoteAdder.h"

#include "InputManager.h"
#include "../Core/Grid.h"

using namespace RedWire;

RedWire::NoteAdder::NoteAdder(InputManager& manager) : Tool(manager) {}

void NoteAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (down) grid->addNote(cell);
}

bool NoteAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Num4);
}
