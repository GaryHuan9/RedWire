#include "Tool.h"
#include "InputManager.h"
#include "../Application.h"

#include <cstring>

using namespace RedWire;

Tool::Tool(InputManager& manager) : manager(manager), grid(manager.application.grid)
{}

void Tool::onDisable()
{}

void Tool::showUI()
{}

const char*& Tool::getName()
{
	const char* name = std::strrchr(typeid(*this).name(), ':') + 1;
	return name;
}
