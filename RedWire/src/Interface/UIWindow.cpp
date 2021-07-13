#include "UIWindow.h"

using namespace RedWire;


RedWire::UIWindow::UIWindow(Application& application) : Component(application)
{}

void RedWire::UIWindow::update()
{
	if (!active) return;

	doUI();
}
