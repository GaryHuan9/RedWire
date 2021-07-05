#pragma once

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Type2.h"

/*
* The main idea of GridView is that the Texture is a fixed size(which is like the scale on how many it should show on the screen)
* Then you could show only a part of the texture, this is going to control the set of the grid color :D
*/
namespace RedWire
{
	class GridView
	{
	private:
		Int2 size;
		sf::Sprite sprite;
		sf::Texture texture;
		sf::Image image;
		sf::View view;
		Float2 topLeftCamPosition;

		float camMoveSpeed;

		std::shared_ptr<Grid> grid;

	public:
		static const Int2 DefaultSize;

		//Change this later into Float2 or FloatRect
		GridView(const Int2& size, const std::shared_ptr<Grid>& grid);

		void update(sf::RenderWindow& renderWindow, const sf::Time& deltaTime);

	private:

		Int2 getTopLeftCellPositionInt() const;

		void updateTexture();
	};
}

