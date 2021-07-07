#include "TestUI.h"
#include <iostream>

using namespace RedWire;

const std::string TestUI::DEFAULT_FONT_FILE_PATH = "Assets/Fonts/JetBrainsMono/JetBrainsMono-Bold.ttf";
const unsigned int TestUI::DEFAULT_UI_CHARACTER_SIZE = 24u;

RedWire::TestUI::TestUI()
{
	if (!defaultFont.loadFromFile(DEFAULT_FONT_FILE_PATH))
	{
		std::cout << "Failed to load font file from path: " << DEFAULT_FONT_FILE_PATH << " are you missing something?\n";
		throw std::exception();
	}
	//else loaded

	wireCountText.setFont(defaultFont);
	gateCountText.setFont(defaultFont);
	deltaTimeText.setFont(defaultFont);

	wireCountText.setCharacterSize(DEFAULT_UI_CHARACTER_SIZE);
	gateCountText.setCharacterSize(DEFAULT_UI_CHARACTER_SIZE);
	deltaTimeText.setCharacterSize(DEFAULT_UI_CHARACTER_SIZE);

	wireCountText.setFillColor(sf::Color::White);
	gateCountText.setFillColor(sf::Color::White);
	deltaTimeText.setFillColor(sf::Color::White);

	gateCountText.move(0.f, (float)DEFAULT_UI_CHARACTER_SIZE);
	deltaTimeText.move(0.f, (float)DEFAULT_UI_CHARACTER_SIZE * 2.0f);
}

void TestUI::update(const Grid& grid, sf::RenderWindow& renderWindow, const sf::Time& deltaTime)
{
	wireCountText.setString(std::string("Wire count: ") + std::to_string(grid.getWiresCount()));
	gateCountText.setString(std::string("Gate count: ") + std::to_string(grid.getGatesCount()));
	deltaTimeText.setString(std::string("FPS: ") + std::to_string(1.0f / deltaTime.asSeconds()));

	renderWindow.draw(wireCountText);
	renderWindow.draw(gateCountText);
	renderWindow.draw(deltaTimeText);
}
