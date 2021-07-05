#include "GridView.h"
#include "Wire.h"

using namespace RedWire;

const Int2 GridView::DefaultSize = Int2{ 40, 20 };

GridView::GridView(const Int2& size, const std::shared_ptr<Grid>& grid) : size(size), grid(grid)
{
	//this is not ideal, will change after I eat
	if (!texture.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y)))
	{
		std::cout << "gridTexture not created!";
	}

	image = texture.copyToImage();

	//we only set the texture once since it stores a pointer instead
	sprite.setTexture(texture);
	gridView.reset(sf::FloatRect(0.f, 0.f, (float)size.x, (float)size.y));
}

void GridView::start()
{
	updateTexture();
}

void GridView::update(const sf::RenderWindow& renderWindow)
{
	//this checks for click left mouse button
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return;

	if (!renderWindow.hasFocus()) return;

	std::cout << "Clicked!\n";

	grid->addWire(Int2{ 3,4 }); //This method is not functional yet!!
}

void GridView::updateTexture()
{
	//updates the grid texture
	for (unsigned int y = 0; y < size.y; y++)
		for (unsigned int x = 0; x < size.x; x++)
		{
			const uint32_t color = grid->getColor(Int2{ static_cast<size_t>(x), static_cast<size_t>(y) });

			//if the cell doesn't exist
			image.setPixel(x, y, sf::Color(color));

			//comment above and uncomment this to see uv rainbow :D
			/*float percentageX = (float) x / size.x,
				percentageY = (float) y / size.y;

			gridImage.setPixel( x, y, sf::Color( (sf::Uint8) ( percentageX * 255 ), (sf::Uint8) ( percentageY * 255 ), (sf::Uint8) 255 ) );*/
		}

	texture.update(image);
}

const Int2& GridView::getSize() const
{
	return size;
}

const sf::View& GridView::getView() const
{
	return gridView;
}

void GridView::display(sf::RenderWindow& renderWindow)
{
	//draws the grid sprite to the view
	renderWindow.draw(sprite);
}

