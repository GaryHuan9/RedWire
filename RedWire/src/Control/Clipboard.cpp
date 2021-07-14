#include "Clipboard.h"
#include "Tool.h"
#include "InputManager.h"
#include "../Type2.h"
#include "../Core/Grid.h"
#include "../Core/IdField.h"
#include "../Application.h"
#include "../Interface/GridView.h"

#include "imgui.h"
#include <istream>
#include <ostream>
#include <memory>
#include <math.h>

using namespace RedWire;

Clipboard::Clipboard(InputManager& manager) : Tool(manager)
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
					Int2 min = startCell.min(cell);
					Int2 max = startCell.max(cell);

					buffer.setSize(max - min + Int2(1));
					buffer.copyFrom(min, *grid);

					isCopying = false;

					if (mode == Mode::cut)
					{
						for (int32_t y = min.y; y <= max.y; y++)
						{
							for (int32_t x = min.x; x <= max.x; x++)
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
				if (buffer.getSize() == Int2(0)) break;
				buffer.pasteTo(cell, *grid);

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

	Int2 size = buffer.getSize();

	if (size != Int2(0))
	{
		ImGui::Text("Clipboard %u x %u", size.x, size.y);

		ImGui::SameLine(); if (ImGui::SmallButton("Clear")) buffer.setSize(Int2(0));

		ImGui::Text("Rotate", size.x, size.y);

		ImGui::SameLine(); if (ImGui::SmallButton("Forward")) buffer.rotatePositive();
		ImGui::SameLine(); if (ImGui::SmallButton("Backward")) buffer.rotateNegative();
		ImGui::SameLine(); if (ImGui::SmallButton("180")) buffer.rotate180();
	}

	if (isCopying)
	{
		Int2 min = startCell.min(lastCell);
		Int2 max = startCell.max(lastCell);

		Int2 delta = max - min + Int2(1);
		ImGui::Text("Copying %u x %u", delta.x, delta.y);
	}
}

void Clipboard::readFrom(std::istream& stream)
{
	buffer.readFrom(stream);
}

bool Clipboard::writeTo(std::ostream& stream)
{
	if (buffer.getSize() == Int2(0)) return false;

	buffer.writeTo(stream);
	return true;
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
				const Int2 min = startCell.min(lastCell);
				const Int2 max = startCell.max(lastCell);

				const Int2 size = max - min + Int2(1);

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
			Int2 size = buffer.getSize();

			if (size == Int2(0))
			{
				view.setPreviewSize(Int2(0));
				break;
			}

			view.setPreviewMin(lastCell);
			view.setPreviewSize(size);

			for (int32_t y = 0; y < size.y; y++)
			{
				for (int32_t x = 0; x < size.x; x++)
				{
					const Int2 position(x, y);
					view.setPreviewColor(position, buffer.getColor(position));
				}
			}

			break;
		}
	}
}