#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "../Core/Grid.h"

namespace RedWire
{
	struct Application;

	class TestUI
	{
	private:
		static const std::string DEFAULT_FONT_FILE_PATH;     //= "Assets/Fonts/JetBrainsMono/JetBrainsMono-Bold.ttf";
		static const uint32_t DEFAULT_UI_CHARACTER_SIZE; //= 24u;
		static const float FPS_UPDATE_INTERVAL;				 // 1.f;

		sf::Font defaultFont;

		sf::Text tileCountText;
		sf::Text wireCountText;
		sf::Text gateCountText;
		sf::Text deltaTimeText;

		// == FPS ==
		uint32_t frameCount;
		float currentInterval;
		float lastAverageFPS;

		Application& application;

	private:
		void setDefault(sf::Text& text);

	public:
		TestUI(Application& application);

		void update();
	};
}

