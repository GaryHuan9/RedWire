#include "GridView.h"
#include "../Application.h"

#include <iostream>
#include <string>

using namespace RedWire;
using namespace sf;

GridView::GridView(Application& application) : application(application), display(), texture(), lines(PrimitiveType::Triangles)
{

}

//For some reason the view occationally jitter randomly while we move
//However, the artifact can be avoided by going into fullscreen mode

void GridView::update()
{
	if (texture.getSize() == Vector2u(0, 0)) return;

	Float2 windowSize = to_type2(float, application.getSize());
	Float2 textureSize = to_type2(float, texture.getSize());

	Float2 density = windowSize / (viewMax - viewMin);
	Float2 offset = (Float2)cellMin - viewMin;

	Float2 position = offset * density;
	Float2 dimension = textureSize * density;

	display.setPosition(Vector2f(position.x, position.y));
	display.setSize(Vector2f(dimension.x, dimension.y));

	uint32_t* colors = (uint32_t*)bytes.get();
	Vector2u size = texture.getSize();

	for (uint32_t y = 0u; y < size.y; y++)
	{
		for (uint32_t x = 0u; x < size.x; x++)
		{
			uint32_t color = application.grid->getColor(cellMin + Int2(x, y));
			colors[static_cast<size_t>(y) * size.x + x] = color;
		}
	}

	texture.update(bytes.get());
	application.draw(display);

	if (!thickness) return;

	lines.clear(); //This doesn't deallocate memory

	static const Color lineColor = Color(85, 85, 85, 30);

	//Horizontal lines
	for (uint32_t y = 0u; y < size.y; y++)
	{
		float targetThickness = thickness * density.y;

		float endX = position.x + dimension.x;
		float centerY = position.y + y * density.y;

		float topY = centerY - targetThickness * .5f;
		float bottomY = centerY + targetThickness * .5f;

		sf::Vertex topLeft(Vector2f(position.x, topY), lineColor);
		sf::Vertex topRight(Vector2f(endX, topY), lineColor);
		sf::Vertex bottomLeft(Vector2f(position.x, bottomY), lineColor);
		sf::Vertex bottomRight(Vector2f(endX, bottomY), lineColor);

		lines.append(bottomRight);
		lines.append(bottomLeft);
		lines.append(topLeft);

		lines.append(bottomRight);
		lines.append(topLeft);
		lines.append(topRight);
	}

	//Vertical lines
	for (uint32_t x = 0u; x < size.x; x++)
	{
		float targetThickness = thickness * density.x;

		float endY = position.y + dimension.y;
		float centerX = position.x + x * density.x;

		float rightX = centerX - targetThickness * .5f;
		float leftX = centerX + targetThickness * .5f;

		sf::Vertex topLeft(Vector2f(leftX, position.y), lineColor);
		sf::Vertex topRight(Vector2f(rightX, position.y), lineColor);
		sf::Vertex bottomLeft(Vector2f(leftX, endY), lineColor);
		sf::Vertex bottomRight(Vector2f(rightX, endY), lineColor);

		lines.append(bottomRight);
		lines.append(bottomLeft);
		lines.append(topLeft);

		lines.append(bottomRight);
		lines.append(topLeft);
		lines.append(topRight);
	}

	application.draw(lines);
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

	Int2 cornerMin = Int2((min - epsilon).getFloor());
	Int2 cornerMax = Int2((max + epsilon).getCeil());

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
	}
}

Float2 GridView::getPosition(const Float2& position) const
{
	Float2 windowSize = to_type2(float, application.getSize());
	return position / windowSize * (viewMax - viewMin) + viewMin;
}