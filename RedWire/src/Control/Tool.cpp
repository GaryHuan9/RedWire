#include "Tool.h"
#include "InputManager.h"
#include "../Application.h"
#include "../Interface/GridView.h"

#include <cstring>

using namespace RedWire;

Tool::Tool(InputManager& manager) : manager(manager), grid(manager.application.grid)
{}

void Tool::onDisable()
{
	GridView& view = manager.application.find<GridView>();
	view.setPreviewSize(Int2(0));
}

void Tool::showUI()
{}

const char* Tool::getDescription()
{
	return "No description for this tool yet.";
}

const char* Tool::getName()
{
	//const char* name = ;
	return std::strrchr(typeid(*this).name(), ':') + 1;
}
