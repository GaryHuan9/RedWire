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

	setDefault(wireCountText);
	setDefault(gateCountText);
	setDefault(deltaTimeText);

	gateCountText.move(0.f, (float)DEFAULT_UI_CHARACTER_SIZE);
	deltaTimeText.move(0.f, (float)DEFAULT_UI_CHARACTER_SIZE * 2.0f);

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
	wireCountText.setString(std::string("Wire count: ") + std::to_string(application.grid.getWiresCount()));
	gateCountText.setString(std::string("Gate count: ") + std::to_string(application.grid.getGatesCount()));
	deltaTimeText.setString(std::string("FPS: ") + std::to_string(lastAverageFPS));

	application.draw(wireCountText);
	application.draw(gateCountText);
	application.draw(deltaTimeText);
}
