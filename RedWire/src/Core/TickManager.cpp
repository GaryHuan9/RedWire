#include "TickManager.h"
#include "SFML/System.hpp"
#include "../Application.h"
#include "../Core/Grid.h"
#include "../Control/InputManager.h"

using namespace RedWire;
using namespace sf;

TickManager::TickManager(Application& application) : Component(application) {}

void TickManager::update()
{
	switch (mode)
	{
		case TickManager::Mode::perSecond:
		{
			Time interval = seconds(1.0f / magnitude);
			timeRemain += application.getDeltaTime();

			while (timeRemain > interval)
			{
				timeRemain -= interval;
				updateSingle();
			}

			break;
		}
		case TickManager::Mode::perFrame:
		{
			updateGrid();
			break;
		}
		case TickManager::Mode::manual:
		{
			bool old = manualPressed;

			manualPressed = InputManager::isPressed(Keyboard::Space);
			if (old != manualPressed && manualPressed) updateGrid();

			break;
		}
	}
}

void TickManager::updateGrid()
{
	for (size_t i = 0; i < magnitude; i++) updateSingle();
}

void TickManager::updateSingle()
{
	application.grid->update();
}
