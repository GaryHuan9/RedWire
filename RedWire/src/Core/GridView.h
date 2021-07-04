#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <assert.h>
#include "Grid.h"

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
		sf::Texture gridTexture;
		sf::Sprite gridSprite;
		sf::Image gridImage;
		std::shared_ptr<Grid> grid;

	public:
		static const Int2 DefaultSize;

		//Change this later into Float2 or FloatRect
		GridView( const Int2& size, const std::shared_ptr<Grid>& grid );

		void start();

		void update( const sf::RenderWindow& renderWindow );

		void updateTexture();

		//Int2 GetSize() const;

		const Int2& getSize() const;

		void display( sf::RenderWindow& renderWindow );
	};
}

