#include "GridView.h"
#include "../Application.h"

#include <iostream>
#include <string>

using namespace RedWire;
using namespace sf;

GridView::GridView(Application& application) : application(application), background(), display(), texture(), lines(PrimitiveType::Triangles)
{
	background.setFillColor(Color(0x000002FFu));
	background.setPosition(Vector2f(0.0f, 0.0f));
}

//For some reason the view occationally jitter randomly while we move
//However, the artifact can be avoided by going into fullscreen mode

int32_t repeat(const int32_t& value, const int32_t& length)
{
	int result = value % length;
	return result < 0 ? result + length : result;
}

void GridView::update()
{
	if (texture.getSize() == Vector2u(0, 0)) return;

	Float2 windowSize = to_type2(float, application.getSize());
	Float2 textureSize = to_type2(float, texture.getSize());

	Float2 density = windowSize / (viewMax - viewMin);
	Float2 offset = (Float2)cellMin - viewMin;

	Float2 position = offset * density;
	Float2 dimension = textureSize * density;

	background.setSize(Vector2f(windowSize.x, windowSize.y));
	application.draw(background);

	uint32_t* colors = (uint32_t*)bytes.get();
	Vector2u size = texture.getSize();

	if (lineThickness > 0.0f)
	{
		lines.clear(); //This doesn't deallocate memory

		static const uint32_t interval = 8;
		static const Color lineColor = Color(42, 42, 45, 16);

		//Not a super good solution for alpha blending
		//But at least it works for a two levels

		Float2 alpha = density / lineThickness;
		alpha = Float2(1.0f).min(alpha / 28.0f);

		//Horizontal lines
		for (int32_t y = cellMin.y; y < cellMax.y; y++)
		{
			bool isBold = repeat(y, interval) == 0;
			float endX = position.x + dimension.x;

			float centerY = position.y + (y - cellMin.y) * density.y;

			float topY = centerY - lineThickness;
			float bottomY = centerY + lineThickness;

			Color color = lineColor;
			if (!isBold) color.a *= alpha.y;

			sf::Vertex topLeft(Vector2f(position.x, topY), color);
			sf::Vertex topRight(Vector2f(endX, topY), color);
			sf::Vertex bottomLeft(Vector2f(position.x, bottomY), color);
			sf::Vertex bottomRight(Vector2f(endX, bottomY), color);

			lines.append(bottomRight);
			lines.append(bottomLeft);
			lines.append(topLeft);

			lines.append(bottomRight);
			lines.append(topLeft);
			lines.append(topRight);
		}

		//Vertical lines
		for (int32_t x = cellMin.x; x < cellMax.x; x++)
		{
			bool isBold = repeat(x, interval) == 0;
			float endY = position.y + dimension.y;

			float centerX = position.x + (x - cellMin.x) * density.x;

			float rightX = centerX - lineThickness;
			float leftX = centerX + lineThickness;

			Color color = lineColor;
			if (!isBold) color.a *= alpha.x;

			sf::Vertex topLeft(Vector2f(leftX, position.y), color);
			sf::Vertex topRight(Vector2f(rightX, position.y), color);
			sf::Vertex bottomLeft(Vector2f(leftX, endY), color);
			sf::Vertex bottomRight(Vector2f(rightX, endY), color);

			lines.append(bottomRight);
			lines.append(bottomLeft);
			lines.append(topLeft);

			lines.append(bottomRight);
			lines.append(topLeft);
			lines.append(topRight);
		}

		application.draw(lines);
	}

	for (uint32_t y = 0u; y < size.y; y++)
	{
		for (uint32_t x = 0u; x < size.x; x++)
		{
			uint32_t color = application.grid->getColor(cellMin + Int2(x, y));
			colors[static_cast<size_t>(y) * size.x + x] = color;
		}
	}

	display.setPosition(Vector2f(position.x, position.y));
	display.setSize(Vector2f(dimension.x, dimension.y));

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