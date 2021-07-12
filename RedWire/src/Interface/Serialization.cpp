#include "Toolbox.h"
#include "Serialization.h"
#include "../Application.h"
#include "../Core/Grid.h"
#include "../Control/InputManager.h"

#include "imgui.h"
#include <filesystem>
#include <fstream>

using namespace RedWire;

namespace fs = std::filesystem;

Serialization::Serialization(Toolbox& toolbox) : Section(toolbox), filePath()
{}

void Serialization::show()
{
	if (!ImGui::CollapsingHeader("Serialization"))
	{
		if (!message.empty()) message.clear();
		return;
	}

	ImGui::SliderInt("Mode", &mode, 0, 1, mode ? "Save" : "Load");

	//not yet
	if (ImGui::BeginListBox("Save Files"))
	{
		if (fs::exists(defaultSaveDirectory))
		{
			for (auto& file : fs::directory_iterator(defaultSaveDirectory))
			{
				const auto& path = file.path();
				const std::string fileName = path.filename().string();

				if (!ImGui::Selectable(fileName.c_str())) continue;

				const std::string pathString = path.string();

				filePath.fill('\0');

				for (size_t i = 0ull; i < pathString.size(); i++)
					filePath[i] = pathString[i];
			}
		}
		ImGui::EndListBox();
	}

	ImGui::InputText("File path", filePath.data(), filePath.size());

	if (ImGui::Button("DO IT"))
	{
		if (mode == 0) // load
		{
			if (filePath[0] == '\0') message = "Loading nothing is unacceptable!";
			else
			{
				std::ifstream stream(filePath.data(), std::ifstream::binary);

				if (stream.fail()) message = "failed to open path!";
				else
				{
					auto& manager = toolbox.application.find<InputManager>();

					toolbox.application.grid = Area::readFrom(stream, manager.viewCenter, manager.viewExtend);
					message = "Successfully loaded save file!";
				}
			}
		}
		else if (mode == 1) // save
		{
			if (filePath[0] == '\0') message = "Loading nothing is unacceptable!";
			else
			{
				std::ofstream stream(filePath.data(), std::ofstream::trunc | std::ofstream::binary);

				if (stream.fail()) message = "failed to open path!";
				else
				{
					auto& manager = toolbox.application.find<InputManager>();

					toolbox.application.grid->writeTo(stream, manager.viewCenter, manager.viewExtend);
					message = "Successfully saved file!";
				}
			}
		}
		else
		{
			throw std::exception("You did the impossible!");
		}
	}

	if (!message.empty())
	{
		ImGui::SameLine(0.f, 5.f);
		ImGui::Text(message.c_str());
	}
}
