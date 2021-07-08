#include "AreaSerializer.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"

#include <fstream>
#include <math.h>

using namespace RedWire;

AreaSerializer::AreaSerializer(InputManager& manager) : Tool(manager)
{}

void AreaSerializer::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	if (!changed) return;
	if (!down)
	{
		const std::string& path = "save";
		Int2 difference = startCell - cell;

		if (std::abs(difference.x) > std::abs(difference.y))
		{
			//Write
			std::ofstream stream(path, std::ofstream::trunc | std::ofstream::binary);
			if (stream.fail()) throw std::exception(("Failed to open " + path).c_str());

			grid->writeTo(stream, manager.viewCenter, manager.viewExtend);
		}
		else if (difference != Int2(0, 0))
		{
			//Read
			std::ifstream stream(path, std::ifstream::binary);
			if (stream.fail()) throw std::exception(("Failed to open " + path).c_str());

			grid = Area::readFrom(stream, manager.viewCenter, manager.viewExtend);
		}
	}
	else startCell = cell;
}

bool AreaSerializer::activationPredicate()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::X);
}
