#include "GridView.h"
#include "Wire.h"
#include "iostream"
#include "CXMath.h"

using namespace RedWire;

const Int2 GridView::DefaultSize = Int2{ 35, 20 };

GridView::GridView(const Int2& size, const std::shared_ptr<Grid>& grid) : size(size), grid(grid), topLeftCamPosition(.0f, .0f), camMoveSpeed(5.f)
{
	//this is not ideal, will change after I eat
	if (!texture.create(static_cast<unsigned int>(size.x + 1u), static_cast<unsigned int>(size.y + 1u)))
	{
		std::cout << "gridTexture not created!";
	}

	image = texture.copyToImage();

	//we only set the texture once since it stores a pointer instead
	sprite.setTexture(texture);

	view.reset(sf::FloatRect(topLeftCamPosition.x, topLeftCamPosition.y, static_cast<float>(size.x), static_cast<float>(size.y)));

	updateTexture();
}

void GridView::update(sf::RenderWindow& renderWindow, const sf::Time& deltaTime)
{
	if (renderWindow.hasFocus())
	{
		//Click check

		sf::Vector2f mouseOnWorld = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
		Int2 mouseOnGrid = Int2((int)std::floorf(mouseOnWorld.x), (int)std::floorf(mouseOnWorld.y)) + getTopLeftCellPositionInt();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) grid->addWire(mouseOnGrid);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) grid->remove(mouseOnGrid);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) std::cout << mouseOnGrid.x << " " << mouseOnGrid.y << "\n";

		//Move camera
		const float deltaTimeAsSec = deltaTime.asSeconds();

		Float2 movementDelta = Float2
		(
			sf::Keyboard::isKeyPressed(sf::Keyboard::A) * -1.f + sf::Keyboard::isKeyPressed(sf::Keyboard::D) * 1.f,
			sf::Keyboard::isKeyPressed(sf::Keyboard::W) * -1.f + sf::Keyboard::isKeyPressed(sf::Keyboard::S) * 1.f
		).normalize();

		topLeftCamPosition += movementDelta * deltaTime.asSeconds() * camMoveSpeed;

		Float2 resultOffset = Float2(CXUtils::CXMath::fract(topLeftCamPosition.x), CXUtils::CXMath::fract(topLeftCamPosition.y));

		//just checking
		//std::cout << "cam top left position: " << resultOffset.x << ", " << resultOffset.y << "\n";

		//view.reset(sf::FloatRect(resultCamPos.x, resultCamPos.y, resultCamPos.x + size.x, resultCamPos.y + size.y));

		Float2 center((float)size.x / 2.f, (float)size.y / 2.f);

		Float2 resultCenter = center + resultOffset;

		view.setCenter(resultCenter.x, resultCenter.y);

		renderWindow.setView(view);
	}

	updateTexture();
	renderWindow.draw(sprite);
}

void GridView::updateTexture()
{
	//offset from top left, which is where the camera position will be
	Int2 topLeftCellPosInt = getTopLeftCellPositionInt();

	//updates the grid texture
	for (unsigned int y = 0; y < size.y + 1u; y++)
	{
		for (unsigned int x = 0; x < size.x + 1u; x++)
		{
			const uint32_t color = grid->getColor(topLeftCellPosInt + Int2{ static_cast<int>(x), static_cast<int>(y) });

			image.setPixel(x, y, sf::Color(color));

			//comment above and uncomment this to see uv rainbow >:(
			float percentageX = CXUtils::CXMath::fract((float)(topLeftCellPosInt.x + x) / size.x);
			float percentageY = CXUtils::CXMath::fract((float)(topLeftCellPosInt.y + y) / size.y);

			//image.setPixel(x, y, sf::Color((sf::Uint8)(percentageX * 255), (sf::Uint8)(percentageY * 255), (sf::Uint8)255));
		}
	}

	texture.update(image);
}

Int2 GridView::getTopLeftCellPositionInt() const
{
	auto value = topLeftCamPosition.getFloored();

	return Int2(static_cast<size_t>(value.x), static_cast<size_t>(value.y));
}
