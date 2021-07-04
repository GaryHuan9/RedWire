#include "GridView.h"

using namespace RedWire;

const Int2 GridView::DefaultSize = Int2 { 40, 20 };

GridView::GridView( const Int2& size, const std::shared_ptr<Grid>& grid ) : size( size ), grid( grid )
{
	//this is not ideal, will change after I eat
	if ( !gridTexture.create( static_cast<unsigned int>( size.x ), static_cast<unsigned int>( size.y ) ) )
	{
		std::cout << "gridTexture not created!";
	}

	gridImage = gridTexture.copyToImage();

	//we only set the texture once since it stores a pointer instead
	gridSprite.setTexture( gridTexture );
}

void GridView::start()
{
	updateTexture();
}

void RedWire::GridView::update( const sf::RenderWindow& renderWindow )
{
	//this checks for click left mouse button
	if ( !sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) ) return;

	if ( !renderWindow.hasFocus() ) return;

	std::cout << "Clicked!\n";
}

void GridView::updateTexture()
{
	//updates the grid texture
	for ( unsigned int y = 0; y < size.y; y++ )
		for ( unsigned int x = 0; x < size.x; x++ )
		{
			auto resultCell = ( *grid )[Int2 { static_cast<size_t>( x ), static_cast<size_t>( y ) }];

			//if the cell doesn't exist
			if ( resultCell == nullptr )
			{
				gridImage.setPixel( x, y, sf::Color::Black );
				continue;
			}

			gridImage.setPixel( x, y, sf::Color( resultCell->getColor() ) );

			//comment above and uncomment this to see uv rainbow :D
			/*float percentageX = (float) x / size.x,
				percentageY = (float) y / size.y;

			gridImage.setPixel( x, y, sf::Color( (sf::Uint8) ( percentageX * 255 ), (sf::Uint8) ( percentageY * 255 ), (sf::Uint8) 255 ) );*/
		}

	gridTexture.update( gridImage );
}

const Int2& GridView::getSize() const
{
	return size;
}

void GridView::display( sf::RenderWindow& renderWindow )
{
	//draws the grid sprite to the view
	renderWindow.draw( gridSprite );
}

