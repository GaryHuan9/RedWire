#include "GridView.h"
#include "Wire.h"
#include "iostream"
#include "CXMath.h"

using namespace RedWire;

const Int2 GridView::DefaultSize = Int2{ 35, 20 };

GridView::GridView(const Int2& size, const std::shared_ptr<Grid>& grid) : size(size), grid(grid), topLeftCamPosition(.0f, .0f), camMoveSpeed(5.f),
zoomLevel(1.f), zoomMin(.2f), zoomMax(5.f)
{
	resize(size);

	//we only set the texture once since it stores a pointer instead
	sprite.setTexture(texture);

	cameraView.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));

	updateTexture();
}

void GridView::onAppEventPoll(const sf::Event& appEvent)
{
	//scroll check

	if (appEvent.type == sf::Event::MouseWheelScrolled)
	{
		int zoomDelta = (int)appEvent.mouseWheelScroll.delta; // between 0 and 1

		float lastZoomLevel = zoomLevel;

		zoomLevel = CXUtils::CXMath::clamp(zoomLevel - zoomDelta, zoomMin, zoomMax);

		int zoomLevelDelta = std::ceil(zoomLevel - lastZoomLevel);

		Int2 addAmountInt(1, 1);

		Int2 newSizeInt(size.x + addAmountInt.x * 2, size.y + addAmountInt.y * 2);

		Float2 newSizeFloat((float)newSizeInt.x, (float)newSizeInt.y);

		cameraView.setSize(newSizeFloat.x, newSizeFloat.y);

		topLeftCamPosition -= Float2((float)addAmountInt.x, (float)addAmountInt.y);

		resize(newSizeInt);
	}
}

void GridView::update(sf::RenderWindow& renderWindow, const sf::Time& deltaTime)
{
	if (renderWindow.hasFocus())
	{
		//Adding selection (TODO: We should probably move this to somewhere nice and add a UI)
		/**/ if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) selectedAdd = 0;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) selectedAdd = 1;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) selectedAdd = 2;

		//Click check
		sf::Vector2f mouseOnWorld = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
		Int2 mouseOnGrid = Int2((int)std::floorf(mouseOnWorld.x), (int)std::floorf(mouseOnWorld.y)) + getTopLeftCellPositionInt();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			switch (selectedAdd)
			{
				case 0: grid->addWire(mouseOnGrid); break;
				case 1: grid->addGate(mouseOnGrid); break;
				case 2: grid->addJoin(mouseOnGrid); break;
			}

			//std::cout << mouseOnGrid.x << " " << mouseOnGrid.y << std::endl;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) grid->remove(mouseOnGrid);

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

		Float2 center((float)size.x / 2.f, (float)size.y / 2.f);

		Float2 resultCenter = center + resultOffset;

		cameraView.setCenter(resultCenter.x, resultCenter.y);

		renderWindow.setView(cameraView);
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

void GridView::resize(const Int2& newSize)
{
	UInt2 newTextureSize(static_cast<unsigned int>(newSize.x + 1), static_cast<unsigned int>(newSize.y + 1));

	if (!texture.create(newTextureSize.x, newTextureSize.y))
	{
		std::cout << "gridTexture not created and resized to the target size: " << newSize.x << ", " << newSize.y << "\n";
		throw std::exception();
	}

	image = texture.copyToImage(); // reset the image also

	size = newSize;
	std::cout << "new size: " << size.x << ", " << size.y << '\n';

	//sprite.setTexture(texture, true); <-- wasteful! NO

	//because the stupid sprite doesn't know that the texture rect is changed, thus we need to tell it to update
	sprite.setTextureRect(sf::IntRect(0, 0, newTextureSize.x, newTextureSize.y));

	//we don't assign to the sprite here because it's assigned as a pointer,
	//so whatever this texture is been resized to any size it's still the same :D
}

Int2 GridView::getTopLeftCellPositionInt() const
{
	auto value = topLeftCamPosition.getFloored();

	return Int2(static_cast<size_t>(value.x), static_cast<size_t>(value.y));
}
