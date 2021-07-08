#include "TestUI.h"
#include <iostream>

#include "../Application.h"

using namespace RedWire;

const std::string TestUI::DEFAULT_FONT_FILE_PATH = "Assets/Fonts/JetBrainsMono/JetBrainsMono-Bold.ttf";
const unsigned int TestUI::DEFAULT_UI_CHARACTER_SIZE = 24u;
const float TestUI::FPS_UPDATE_INTERVAL = 1.f;

void TestUI::setDefault(sf::Text& text)
{
	text.setFont(defaultFont);
	text.setCharacterSize(DEFAULT_UI_CHARACTER_SIZE);
	text.setFillColor(sf::Color::White);
}

TestUI::TestUI(Application& application) : application(application), currentInterval(0.f), frameCount(0u), lastAverageFPS(0.f)
{
	if (!defaultFont.loadFromFile(DEFAULT_FONT_FILE_PATH))
	{
		std::cout << "Failed to load font file from path: " << DEFAULT_FONT_FILE_PATH << " are you missing something?\n";
		throw std::exception();
	}
	//else loaded

	setDefault(tileCountText);
	setDefault(wireCountText);
	setDefault(gateCountText);
	setDefault(deltaTimeText);

	wireCountText.move(0.f, (float)DEFAULT_UI_CHARACTER_SIZE * 1.0f);
	gateCountText.move(0.f, (float)DEFAULT_UI_CHARACTER_SIZE * 2.0f);
	deltaTimeText.move(0.f, (float)DEFAULT_UI_CHARACTER_SIZE * 3.0f);
}

void TestUI::update()
{
	// == calculate FPS ==

	if (currentInterval >= FPS_UPDATE_INTERVAL)
	{
		lastAverageFPS = (float)frameCount / currentInterval;

		frameCount = 0u;
		currentInterval = 0.f;
	}

	frameCount++;

	currentInterval += application.getDeltaTime().asSeconds();

	// == draw text ==
	tileCountText.setString(std::string("Tile count: ") + std::to_string(application.grid->getTileCount()));
	wireCountText.setString(std::string("Wire count: ") + std::to_string(application.grid->getWireCount()));
	gateCountText.setString(std::string("Gate count: ") + std::to_string(application.grid->getGateCount()));
	deltaTimeText.setString(std::string("FPS: ") + std::to_string(lastAverageFPS));

	application.draw(tileCountText);
	application.draw(wireCountText);
	application.draw(gateCountText);
	application.draw(deltaTimeText);
}
