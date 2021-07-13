#include "Clipboard.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"
#include "../Application.h"
#include "../Interface/GridView.h"

#include "imgui.h"
#include <iterator>
#include <sstream>
#include <memory>
#include <math.h>

using namespace RedWire;

Clipboard::Clipboard(InputManager& manager) : Tool(manager), buffer(std::make_unique<std::stringstream>())
{}

void Clipboard::update(const Float2& position, const Int2& cell, const bool& down, const bool& changed)
{
	Mode oldMode = mode;
	Int2 oldCell = lastCell;

	if (InputManager::isPressed(sf::Keyboard::C)) mode = Mode::copy;
	if (InputManager::isPressed(sf::Keyboard::X)) mode = Mode::cut;
	if (InputManager::isPressed(sf::Keyboard::V)) mode = Mode::paste;

	lastCell = cell;

	if (changed && down)
	{
		switch (mode)
		{
			case Mode::copy:
			case Mode::cut:
			{
				if (isCopying)
				{
					buffer->seekp(0);
					buffer->str("");

					Int2 min = startCell.min(cell);
					Int2 max = startCell.max(cell);

					copiedSize = max - min + Int2(1);
					grid->writeTo(*buffer, min, max);

					previewGrid.reset();
					isCopying = false;

					if (mode == Mode::cut)
					{
						for (int y = min.y; y <= max.y; y++)
						{
							for (int x = min.x; x <= max.x; x++)
							{
								grid->remove(Int2(x, y));
							}
						}
					}
				}
				else
				{
					startCell = cell;
					isCopying = true;
				}

				break;
			}
			case Mode::paste:
			{
				if (copiedSize == Int2(0)) break;

				buffer->seekg(0);
				Area::readFrom(*buffer, *grid, cell);

				break;
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
		|| InputManager::isPressed(sf::Keyboard::X)
		|| InputManager::isPressed(sf::Keyboard::V);
}

void Clipboard::showUI()
{
	int* ptr = reinterpret_cast<int*>(&mode);
	static const char* names[] = { "Copy", "Cut", "Paste" };

	if (ImGui::SliderInt("Mode", ptr, 0, 2, names[*ptr]))
	{
		isCopying = false;
		updatePreview();
	}

	if (copiedSize != Int2(0))
	{
		ImGui::Text("Clipboard %u x %u", copiedSize.x, copiedSize.y);

		ImGui::SameLine();

		if (ImGui::SmallButton("Clear"))
		{
			copiedSize = Int2(0);
			previewGrid.reset();
		}
	}

	if (isCopying)
	{
		Int2 min = startCell.min(lastCell);
		Int2 max = startCell.max(lastCell);

		Int2 delta = max - min + Int2(1);
		ImGui::Text("Copying %u x %u", delta.x, delta.y);
	}
}

std::unique_ptr<Grid> createGrid(std::istream& stream, Int2& size)
{
	stream.seekg(0);

	auto pointer = std::make_unique<Grid>();
	size = Area::readFrom(stream, *pointer, Int2(0));

	return pointer;
}

bool Clipboard::writeTo(std::ostream& stream)
{
	if (copiedSize == Int2(0)) return false;

	if (previewGrid == nullptr) previewGrid = createGrid(*buffer, copiedSize);
	previewGrid->writeTo(stream, Int2(0), copiedSize - Int2(1));

	return true;
}

void Clipboard::readFrom(std::istream& stream)
{
	buffer->seekp(0);
	buffer->str("");

	std::copy
	(
		std::istreambuf_iterator<char>(stream),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(*buffer)
	);

	previewGrid = createGrid(*buffer, copiedSize);
}

void Clipboard::doHelpUI()
{
	ImGui::Text("This could");

	ImGui::SameLine(); ImGui::Button("Copy");
	ImGui::SameLine(); ImGui::Text("|");
	ImGui::SameLine(); ImGui::Button("Cut");
	ImGui::SameLine(); ImGui::Text("|");
	ImGui::SameLine(); ImGui::Button("Paste");
	ImGui::SameLine(); ImGui::Text("a rectangular area of cells\n");

	ImGui::Text("To use [Copy] or [Cut], select the mode and select two corner cells to [Copy] or [Cut]\n\
To use [Paste], select the mode and align the preview then click on the cell to [Paste]");
}

void Clipboard::updatePreview()
{
	GridView& view = manager.application.find<GridView>();

	switch (mode)
	{
		case Mode::copy:
		case Mode::cut:
		{
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

			break;
		}
		case Mode::paste:
		{
			if (copiedSize != Int2(0))
			{
				if (previewGrid == nullptr) previewGrid = createGrid(*buffer, copiedSize);

				view.setPreviewMin(lastCell);
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
			else view.setPreviewSize(Int2(0));

			break;
		}
	}
}