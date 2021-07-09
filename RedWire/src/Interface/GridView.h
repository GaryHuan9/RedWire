#pragma once

#include <SFML/Graphics.hpp>

#include "../Type2.h"
#include "../Core/Grid.h"

namespace RedWire
{
	struct Application;

	/*
	* Main idea of the grid view:
	* 1. it only controls the display of the grid and the manipulation of it
	* 2. it will have 3 methods that are extendable
	*	a. Update
	*	b. Resizing of borders
	*	c. get world position from screen position
	*/
	struct GridView
	{
		GridView(Application& application);

		void update();

		/// <summary>
		/// Assigns this GridView's current view.
		/// </summary>
		void setView(const Float2& min, const Float2& max);

		/// <summary>
		/// Converts a screen position to a world position
		/// based on this GridView's current view.
		/// </summary>
		Float2 getPosition(const Float2& position) const;

		float thickness = .05f;

	private:

		Int2 cellMin;
		Int2 cellMax;

		Float2 viewMin;
		Float2 viewMax;

		sf::RectangleShape display;
		sf::Texture texture;
		std::unique_ptr<sf::Uint8[]> bytes;

		std::vector<sf::RectangleShape> linesNew;

		Application& application;
	};
}


