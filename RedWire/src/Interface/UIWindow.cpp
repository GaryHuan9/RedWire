#include "UIWindow.h"

using namespace RedWire;


UIWindow::UIWindow(Application& application) : Component(application)
{}

void UIWindow::update()
{
	if (!active) return;

	showUI();
}
