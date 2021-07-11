#include "NoteAdder.h"

#include "InputManager.h"
#include "../Core/Grid.h"

using namespace RedWire;

RedWire::NoteAdder::NoteAdder(InputManager& manager) : LineTool(manager) {}

void NoteAdder::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!InputManager::isPressed(sf::Keyboard::LShift))
	{
		if (down)
			grid->addNote(cell);

		return;
	}

	DoLineDraw(down, cell);
}

bool NoteAdder::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::Num4);
}

void NoteAdder::SetLineCell(const Int2& cell)
{
	grid->addNote(cell);
}
