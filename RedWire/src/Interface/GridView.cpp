#include "GridView.h"
#include "../Core/Grid.h"
#include "../Application.h"
#include "../Component.h"

#include <iostream>
#include <string>

using namespace RedWire;
using namespace sf;

GridView::GridView(Application& application) : Component(application), lines(PrimitiveType::Triangles)
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
	if (displayTexture.getSize() == Vector2u(0, 0)) return;

	Float2 windowSize = to_type2(float, application.getSize());
	Float2 textureSize = to_type2(float, displayTexture.getSize());

	Float2 density = windowSize / (viewMax - viewMin);
	Float2 offset = (Float2)cellMin - viewMin;

	Float2 position = offset * density;
	Float2 dimension = textureSize * density;

	background.setSize(Vector2f(windowSize.x, windowSize.y));
	application.draw(background);

	uint32_t* colors = (uint32_t*)displayBytes.get();
	Vector2u size = displayTexture.getSize();

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
			if (!isBold) color.a = (Uint8)(color.a * alpha.y);

			Vertex topLeft(Vector2f(position.x, topY), color);
			Vertex topRight(Vector2f(endX, topY), color);
			Vertex bottomLeft(Vector2f(position.x, bottomY), color);
			Vertex bottomRight(Vector2f(endX, bottomY), color);

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
			if (!isBold) color.a = (Uint8)(color.a * alpha.x);

			Vertex topLeft(Vector2f(leftX, position.y), color);
			Vertex topRight(Vector2f(rightX, position.y), color);
			Vertex bottomLeft(Vector2f(leftX, endY), color);
			Vertex bottomRight(Vector2f(rightX, endY), color);

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

	position += (previewMin - cellMin).toType<float>() * density;
	dimension = to_type2(float, previewSize) * density;

	preview.setPosition(Vector2f(position.x, position.y));
	preview.setSize(Vector2f(dimension.x, dimension.y));

	displayTexture.update(displayBytes.get());
	previewTexture.update(previewBytes.get());

	application.draw(display);
	application.draw(preview);
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
		if (displayTexture.create(newSize.x, newSize.y)) displayBytes = std::make_unique<Uint8[]>(getBytesLength(newSize));
		else throw std::exception(("GridView failed to create display texture: size " + newSize.toString()).c_str());

		display.setTexture(&displayTexture);
		display.setTextureRect(IntRect(0, 0, newSize.x, newSize.y));
	}
}

Float2 GridView::getPosition(const Float2& position) const
{
	Float2 windowSize = to_type2(float, application.getSize());
	return position / windowSize * (viewMax - viewMin) + viewMin;
}

void GridView::setPreviewMin(const Int2& min)
{
	previewMin = min;
}

void GridView::setPreviewSize(const Int2& size)
{
	previewSize = size;

	Int2 textureSize = to_type2(int32_t, previewTexture.getSize());
	if (textureSize == size || Int2(0) == size) return;

	//Resize texture
	if (previewTexture.create(size.x, size.y)) previewBytes = std::make_unique<Uint8[]>(getBytesLength(size));
	else throw std::exception(("GridView failed to create preview texture: size " + size.toString()).c_str());

	preview.setTexture(&previewTexture);
	preview.setTextureRect(IntRect(0, 0, size.x, size.y));
}

void GridView::setPreviewColor(const Int2& position, const uint32_t& color)
{
	size_t index = static_cast<size_t>(position.y) * previewSize.x + position.x;

	union
	{
		uint32_t value;
		char bytes[4];
	} fuse{ color };

	previewBytes[index * 4 + 0] = fuse.bytes[0];
	previewBytes[index * 4 + 1] = fuse.bytes[1];
	previewBytes[index * 4 + 2] = fuse.bytes[2];
	previewBytes[index * 4 + 3] = previewAlpha;
}
