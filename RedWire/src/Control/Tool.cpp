#include "Tool.h"
#include "InputManager.h"
#include "../Application.h"

using namespace RedWire;

Tool::Tool(InputManager& manager) : manager(manager), grid(manager.application.grid)
{}
