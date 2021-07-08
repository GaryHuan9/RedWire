#include "GridView.h"
#include "../Application.h"

#include <iostream>
#include <string>

using namespace RedWire;
using namespace sf;

GridView::GridView(Application& application) : application(application), display(), texture()
{

}

//For some reason the view occationally jitter randomly while we move
//However, the artifact can be avoided by going into fullscreen mode

void GridView::update()
{
	if (texture.getSize() == Vector2u(0, 0)) return;
	
	Float2 windowSize = to_type2(float, application.getSize());// .toType<float>();
	Float2 textureSize = to_type2(float, texture.getSize());

	Float2 density = windowSize / (viewMax - viewMin);
	Float2 offset = (Float2)cellMin - viewMin;

	Float2 position = offset * density;
	Float2 dimension = textureSize * density;

	display.setPosition(Vector2f(position.x, position.y));
	display.setSize(Vector2f(dimension.x, dimension.y));

	uint32_t* colors = (uint32_t*)bytes.get();
	Vector2u size = texture.getSize();

	for (uint32_t y = 0; y < size.y; y++)
	{
		for (uint32_t x = 0; x < size.x; x++)
		{
			uint32_t color = application.grid.getColor(cellMin + Int2(x, y));
			colors[static_cast<size_t>(y) * size.x + x] = color;
		}
	}

	texture.update(bytes.get());
	application.draw(display);
}

size_t getBytesLength(const Int2& size)
{
	return static_cast<size_t>(size.x) * size.y * 4;
}

void GridView::setView(const Float2& min, const Float2& max)
{
	viewMin = min;
	viewMax = max;

	static const Float2 epsilon(0.01f, 0.01f);

	Int2 cornerMin = (min - epsilon).getFloor().toType<int32_t>();
	Int2 cornerMax = (max + epsilon).getCeil().toType<int32_t>();

	Int2 oldSize = cellMax - cellMin;
	Int2 newSize = cornerMax - cornerMin;

	Int2 delta = oldSize - newSize;

	if ((delta.x == 0 || delta.x == 1) && (delta.y == 0 || delta.y == 1))
	{
		cellMin = cornerMin;
		cellMax = cornerMin + oldSize;
	}
	else
	{
		cellMin = cornerMin;
		cellMax = cornerMax;

		//Resize texture
		if (texture.create(newSize.x, newSize.y)) bytes = std::make_unique<Uint8[]>(getBytesLength(newSize));
		else throw std::exception(("GridView failed to create texture: size " + newSize.toString()).c_str());

		display.setTexture(&texture);
		display.setTextureRect(IntRect(0, 0, newSize.x, newSize.y));

		std::cout << "Resized" << oldSize.toString() << " " << newSize.toString() << std::endl;
	}
}

Float2 GridView::getPosition(const Float2& position) const
{
	Float2 windowSize = to_type2(float, application.getSize());
	return position / windowSize * (viewMax - viewMin) + viewMin;
}