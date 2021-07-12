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

Serialization::Serialization(Toolbox& toolbox) : Section(toolbox), fileName()
{}

void Serialization::show()
{
	if (!ImGui::CollapsingHeader("Serialization"))
	{
		message.clear();
		return;
	}

	ImGui::SliderInt("Mode", &mode, 0, 1, mode ? "Clipboard" : "Circuit");

	ImGui::InputText("File name", fileName.data(), fileName.size());

	if (ImGui::BeginCombo("Save Files", "Choose save"))
	{
		if (fs::exists(defaultSaveDirectory))
		{
			for (auto& file : fs::directory_iterator(defaultSaveDirectory))
			{
				const std::string targetFileName = file.path().filename().string();

				if (!ImGui::Selectable(targetFileName.c_str())) continue;

				fileName.fill('\0');

				for (size_t i = 0ull; i < targetFileName.size(); i++)
					fileName[i] = targetFileName[i];
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Load"))
	{
		if (mode == 0)
		{
			if (fileName[0] == '\0') message = "Loading nothing is unacceptable!";
			else
			{
				std::ifstream stream(defaultSaveDirectory + fileName.data(), std::ifstream::binary);

				if (stream.fail()) message = "failed to open path!";
				else
				{
					auto& manager = toolbox.application.find<InputManager>();

					toolbox.application.grid = Area::readFrom(stream, manager.viewCenter, manager.viewExtend);
					message = "Successfully loaded save file!";
				}
			}
		}
		else if (mode == 1)
		{
			//do loading into clipboard
			message = "Not implemented hehe";
		}
		else
		{
			throw std::exception("You did the impossible :D");
		}
	}

	ImGui::SameLine(0.f, 5.f);

	if (ImGui::Button("Save"))
	{
		if (mode == 0)
		{
			if (fileName[0] == '\0') message = "Loading nothing is unacceptable!";
			else
			{
				std::ofstream stream(defaultSaveDirectory + fileName.data(), std::ofstream::trunc | std::ofstream::binary);

				if (stream.fail()) message = "failed to open path!";
				else
				{
					auto& manager = toolbox.application.find<InputManager>();

					toolbox.application.grid->writeTo(stream, manager.viewCenter, manager.viewExtend);
					message = "Successfully saved file!";
				}
			}
		}
		else if (mode == 1)
		{
			//do saving from clipboard
			message = "Not implemented hehe";
		}
		else
		{
			throw std::exception("You did the impossible :D");
		}
	}

	if (!message.empty())
	{
		ImGui::SameLine(0.f, 5.f);
		ImGui::Text(message.c_str());
	}
}
