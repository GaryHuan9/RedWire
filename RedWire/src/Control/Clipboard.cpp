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

void Clipboard::showHelpUI()
{
	ImGui::TextWrapped("The Clipboard can ");

	ImGui::SameLine(); ImGui::SmallButton("Copy");
	ImGui::SameLine(); ImGui::SmallButton("Cut");
	ImGui::SameLine(); ImGui::SmallButton("Paste");
	ImGui::SameLine();

	ImGui::Text(" a rectangular area of cells on the [Grid]");

	ImGui::Text("To Copy or Cut, click on two corners to select all cells underneath and buffer them");
	ImGui::Text("While Pasting, the buffered cells will appear as a preview and click to place them down");

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::Text("Keyboard Shortcut (Copy): C");
	ImGui::Text("Keyboard Shortcut (Cut): X");
	ImGui::Text("Keyboard Shortcut (Paste): V");
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