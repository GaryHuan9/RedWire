#pragma once

#include <SFML/Graphics.hpp>
#include "../Component.h"
#include "../Type2.h"

namespace RedWire
{
	struct Application;

	/*
	* Main idea of the grid view:
	* 1. it only controls the display of the grid and the manipulation of it
	* 2. it will have 3 methods that are extendable
	*	a. Update
	*	b. Resizing of borders
	*	c. get world position from screen position
	*/
	struct GridView : Component
	{
		GridView(Application& application);

		virtual void update() override;

		/// <summary>
		/// Assigns this GridView's current view.
		/// </summary>
		void setView(const Float2& min, const Float2& max);

		/// <summary>
		/// Converts a screen position to a world position
		/// based on this GridView's current view.
		/// </summary>
		Float2 getPosition(const Float2& position) const;

		void setPreviewMin(const Int2& min);
		void setPreviewSize(const Int2& size);

		void setPreviewColor(const Int2& position, const uint32_t& color);

		float lineThickness = 1.8f;
		sf::Uint8 previewAlpha = 84;

	private:

		Int2 cellMin;
		Int2 cellMax;

		Float2 viewMin;
		Float2 viewMax;

		Int2 previewMin;
		Int2 previewSize;

		sf::RectangleShape background;
		sf::RectangleShape display;
		sf::RectangleShape preview;

		std::unique_ptr<sf::Uint8[]> displayBytes;
		std::unique_ptr<sf::Uint8[]> previewBytes;

		sf::Texture displayTexture;
		sf::Texture previewTexture;
		sf::VertexArray lines;
	};
}


