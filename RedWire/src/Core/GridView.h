#pragma once

#include <assert.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"

namespace RedWire
{
	class GridView
	{
	private:
		Int2 size;
		sf::Texture gridTexture;
		std::shared_ptr<Grid> grid;

	public:
		//Change this later into Float2 or FloatRect
		GridView( const Int2& size, const std::shared_ptr<Grid>& grid );

		void Start();

		void UpdateTexture();

		const sf::Texture& GetTexture() const;
	};
}

