#pragma once

#include <SFML/Graphics.hpp>

#include "Type2.h"
#include "Grid.h"
#include "InputManager.h"

namespace RedWire
{
	/*
	* Main idea of the grid view:
	* 1. it only controls the display of the grid and the manipulation of it
	* 2. it will have 3 methods that is extendable
	*	a. Update
	*	b. Resizing of borders
	*	c. get mouse on cell position
	*/
	class GridViewNew
	{
	private:
		sf::Texture texture;
		sf::Image textureImage;
		sf::Sprite displaySprite;
		sf::Sprite previewSprite;
		sf::View cameraView;

		Float2 viewSize;
		Float2 minPosition;

		// == movement ==
		float moveSpeed;

		std::shared_ptr<Grid> grid;

		Int2 getMinBorderInt() const;
		UInt2 getDisplayedGridSize() const;
		void recreateTexture(const UInt2& newSize);

	public:
		GridViewNew(const std::shared_ptr<Grid>& grid, const Float2& minBorder, const Float2& maxBorder);

		void update(sf::RenderWindow& renderWindow, const InputManager& inputManager, const sf::Time& deltaTime);
		void setCenter(const Float2& viewCenter);
		Float2 getViewCenter() const;
		void resetBorder(const Float2& _minBorder, const Float2& _maxBorder);
		Int2 getMouseOnGrid(const sf::RenderWindow& renderWindow) const;
	};
}


