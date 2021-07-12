#include "Clipboard.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"
#include "../Application.h"
#include "../Interface/GridView.h"

#include "imgui.h"
#include <sstream>
#include <memory>
#include <math.h>

using namespace RedWire;

Clipboard::Clipboard(InputManager& manager) : Tool(manager), stream(std::make_unique<std::stringstream>())
{}

void Clipboard::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	int oldMode = mode;
	Int2 oldCell = lastCell;

	if (InputManager::isPressed(sf::Keyboard::C)) mode = 0;
	if (InputManager::isPressed(sf::Keyboard::V)) mode = 1;

	lastCell = cell;

	if (changed && down)
	{
		if (mode)
		{
			//Pasting

			if (copiedSize != Int2(0))
			{
				stream->seekg(0);
				Area::readFrom(*stream, *grid, cell);
			}
		}
		else
		{
			//Copying

			if (isCopying)
			{
				stream->seekp(0);

				Int2 min = startCell.min(cell);
				Int2 max = startCell.max(cell);

				copiedSize = max - min + Int2(1);
				grid->writeTo(*stream, min, max);

				previewGrid.reset();
				isCopying = false;
			}
			else
			{
				startCell = cell;
				isCopying = true;
			}
		}

		updatePreview();
	}
	else if (lastCell != oldCell || mode != oldMode) updatePreview();
}

void Clipboard::onDisable()
{
	isCopying = false;
	Tool::onDisable();
}

bool Clipboard::activationPredicate()
{
	return InputManager::isPressed(sf::Keyboard::C)
		|| InputManager::isPressed(sf::Keyboard::V);
}

void Clipboard::showUI()
{
	if (ImGui::SliderInt("Mode", &mode, 0, 1, mode ? "Paste" : "Copy"))
	{
		isCopying = false;
		updatePreview();
	}
}

void Clipboard::updatePreview()
{
	GridView& view = manager.application.find<GridView>();

	if (mode)
	{
		//Pasting

		if (copiedSize != Int2(0))
		{
			if (previewGrid == nullptr)
			{
				stream->seekg(0);
				previewGrid = Area::readFrom(*stream);

				view.setPreviewSize(copiedSize);

				for (int y = 0; y < copiedSize.y; y++)
				{
					for (int x = 0; x < copiedSize.x; x++)
					{
						const Int2 position(x, y);
						view.setPreviewColor(position, previewGrid->getColor(position));
					}
				}
			}

			view.setPreviewMin(lastCell);
		}
		else view.setPreviewSize(Int2(0));
	}
	else
	{
		//Copying

		static const uint32_t color = 0xFF111113u;

		if (isCopying)
		{
			Int2 min = startCell.min(lastCell);
			Int2 max = startCell.max(lastCell);

			Int2 size = max - min + Int2(1);

			view.setPreviewMin(min);
			view.setPreviewSize(size);

			for (int y = 0; y < size.y; y++)
			{
				for (int x = 0; x < size.x; x++)
				{
					view.setPreviewColor(Int2(x, y), color);
				}
			}
		}
		else
		{
			view.setPreviewMin(lastCell);
			view.setPreviewSize(Int2(1));

			view.setPreviewColor(Int2(0), color);
		}
	}
}