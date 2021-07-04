#include "GridView.h"

using namespace RedWire;

GridView::GridView( const Int2& size, const std::shared_ptr<Grid>& grid ) : size( size ), grid( grid )
{
	//this is not ideal, will change after I eat
	if ( !gridTexture.create( static_cast<unsigned int>( size.x ), static_cast<unsigned int>( size.y ) ) )
	{
		std::cout << "gridTexture not created!";
	}
}

void RedWire::GridView::Start()
{
	UpdateTexture();
}

void RedWire::GridView::UpdateTexture()
{
	//updates the grid texture

	sf::Uint8 pixels[Grid::Tile::size * Grid::Tile::size] {};

	for ( size_t y = 0; y < Grid::Tile::size; y++ )
		for ( size_t x = 0; x < Grid::Tile::size; x++ )
			pixels[y * Grid::Tile::size + x] = ( *grid )[Int2 { x, y }]->getColor();

	gridTexture.update( pixels );
}

const sf::Texture& RedWire::GridView::GetTexture() const
{
	return gridTexture;
}

