#include "GridViewNew.h"

#include <iostream>
#include <string>

#include "CXMath.h"

using namespace RedWire;

Int2 GridViewNew::getMinBorderInt() const
{
	return minPosition.getFloor().toType<int>();
}

UInt2 RedWire::GridViewNew::getDisplayedGridSize() const
{
	return viewSize.getCeil().toType<uint32_t>() + UInt2(1u, 1u);
}

void GridViewNew::recreateTexture(const UInt2& newSize)
{
	if (!texture.create(newSize.x, newSize.y))
	{
		std::cout << "GridView texture failed to create!\n";
		throw std::exception();
	}

	textureImage = texture.copyToImage();
}


GridViewNew::GridViewNew(const std::shared_ptr<Grid>& grid, const Float2& _minBorder, const Float2& _maxBorder) : grid(grid),
minPosition(_minBorder), viewSize(_maxBorder - _minBorder), moveSpeed(10.f)
{
	UInt2 displayedGridSize = getDisplayedGridSize();

	recreateTexture(displayedGridSize);

	displaySprite.setTexture(texture);

	cameraView.reset(sf::FloatRect(minPosition.x, minPosition.y, minPosition.x + viewSize.x, minPosition.y + viewSize.y));
	//cameraView.setSize(viewSize.x, viewSize.y); // somehow this doesn't work
}

void GridViewNew::update(sf::RenderWindow& renderWindow, const InputManager& inputManager, const sf::Time& deltaTime)
{
	// == move view ==

	Float2 movementDelta = inputManager.getMovementDeltaRaw().normalize() * deltaTime.asSeconds();

	Float2 newViewCenter = getViewCenter() + movementDelta * moveSpeed;

	setCenter(newViewCenter);

	// == set view ==
	renderWindow.setView(cameraView);

	// == draw on image ==
	Int2 minBorderInt = getMinBorderInt();

	UInt2 textureSize = getDisplayedGridSize();

	for (uint32_t y = 0u; y < textureSize.y; y++)
	{
		for (uint32_t x = 0u; x < textureSize.x; x++)
		{
			Int2 renderOffset(x, y);

			Int2 renderingGridPos = minBorderInt + renderOffset;

			textureImage.setPixel(x, y, sf::Color(grid->getColor(renderingGridPos)));

			//nothing here :D
			/*Int2 minBorderInt = getMinBorderInt();

			Float2 uvPercentage(
				CXUtils::CXMath::fract((float)(x + minBorderInt.x) / textureSize.x),
				CXUtils::CXMath::fract((float)(y + minBorderInt.y) / textureSize.y)
			);

			textureImage.setPixel(x, y, sf::Color((sf::Uint8)(uvPercentage.x * 255.f), (sf::Uint8)(uvPercentage.y * 255.f), 255u));*/
		}
	}

	texture.update(textureImage);

	// == draw ==
	renderWindow.draw(displaySprite);

	// == do other kinds of stuff ==

	//beware that get mouse on grid must be called after set view :D since this rely's on the conversion of the mouse position
	//I do have another method to fix it tho :D it requires a little bit more steps
	Int2 mouseOnGrid = getMouseOnGrid(renderWindow);

	//remove this to another class somehow :D
	if (renderWindow.hasFocus())
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			grid->addWire(mouseOnGrid);
		}
	}

	//std::cout << getMouseOnGrid(renderWindow).toString() << '\n';
}

void GridViewNew::setCenter(const Float2& viewCenter)
{
	Float2 halfViewSize = viewSize / 2.f;
	minPosition = viewCenter - halfViewSize;

	cameraView.setCenter(viewCenter.x, viewCenter.y);

	Float2 minPosFloored = minPosition.getFloor();

	displaySprite.setPosition(minPosFloored.x, minPosFloored.y);
}

Float2 RedWire::GridViewNew::getViewCenter() const
{
	return minPosition + (viewSize / 2.f);
}

void GridViewNew::resetBorder(const Float2& _minBorder, const Float2& _maxBorder)
{
	minPosition = _minBorder;
	viewSize = _maxBorder - minPosition;

	UInt2 textureSize = getDisplayedGridSize();

	recreateTexture(textureSize);

	displaySprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));

	cameraView.reset(sf::FloatRect(minPosition.x, minPosition.y, minPosition.x + viewSize.x, minPosition.y + viewSize.y));
}

Int2 GridViewNew::getMouseOnGrid(const sf::RenderWindow& renderWindow) const
{
	sf::Vector2f mouseOnWorld = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));

	return Int2((int)std::floorf(mouseOnWorld.x), (int)std::floorf(mouseOnWorld.y));// + getMinBorderInt(); <- we don't need this since we move the sprite also to the camera so it's technically really the world position :D
}