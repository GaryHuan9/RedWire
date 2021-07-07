#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../Core/Grid.h"

namespace RedWire
{
	class TestUI
	{
	private:
		static const std::string DEFAULT_FONT_FILE_PATH;     //= "Assets/Fonts/JetBrainsMono/JetBrainsMono-Bold.ttf";
		static const unsigned int DEFAULT_UI_CHARACTER_SIZE; //= 24u;

		sf::Font defaultFont;

		sf::Text wireCountText;
		sf::Text gateCountText;
		sf::Text deltaTimeText;

	public:
		TestUI();

		void update(const Grid& grid, sf::RenderWindow& renderWindow, const sf::Time& deltaTime);
	};
}

