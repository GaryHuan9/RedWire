#include "Clipboard.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

#include <sstream>
#include <memory>
#include <math.h>

using namespace RedWire;

Clipboard::Clipboard(InputManager& manager) : Tool(manager), stream(std::make_unique<std::stringstream>())
{}

void Clipboard::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!changed) return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		if (down) startCell = cell;
		else
		{
			hasCopied = true;
			stream->seekp(0);

			Int2 min = startCell.min(cell);
			Int2 max = startCell.max(cell);
			grid->writeTo(*stream, min, max);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
	{
		if (down && hasCopied)
		{
			stream->seekg(0);
			Area::readFrom(*stream, *grid, cell);
		}
	}
}

bool Clipboard::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);
}
