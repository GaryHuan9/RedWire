#include "Toolbox.h"
#include "Serialization.h"
#include "../Application.h"
#include "../Core/Grid.h"
#include "../Control/InputManager.h"
#include "../Control/Clipboard.h"

#include "imgui.h"
#include "imgui_internal.h"
#include <fstream>

#include "../Control/SaveManager.h"

using namespace RedWire;

namespace fs = std::filesystem;

Serialization::Serialization(Toolbox& toolbox) : Section(toolbox)//, fileName()
{}

void Serialization::show()
{
	if (!ImGui::CollapsingHeader("Serialization", ImGuiTreeNodeFlags_DefaultOpen))
	{
		message.clear();
		return;
	}

	SaveManager& saveManager = toolbox.application.find<SaveManager>();

	std::array<char, 100ull>& fileName = saveManager.getLast();
	const fs::path& savePath = saveManager.savePath;

	int* ptr = reinterpret_cast<int*>(&mode);

	static const char* names[] = { "Circuit", "Clipboard" };
	ImGui::SliderInt("Mode", ptr, 0, 1, names[*ptr]);

	ImGui::InputText("Save Name", fileName.data(), fileName.size(), ImGuiInputTextFlags_AutoSelectAll);

	if (ImGui::BeginCombo("Saves", fileName.data()))
	{
		if (fs::exists(savePath))
		{
			for (auto& file : saveManager.getSavesIterator())
			{
				const auto& target = file.path().filename().string();
				if (!ImGui::Selectable(target.c_str())) continue;

				message.clear();

				saveManager.setLast(target);
			}
		}
		else
		{
			bool created = fs::create_directory(savePath);
			if (!created) message = "Failed to create path";
		}

		ImGui::EndCombo();
	}

	bool hasName = fileName[0] != '\0';
	//TODO: Use PushDisable when we update ImGui to gray out the buttons
	//understood :D

	if (ImGui::Button("Load") && hasName)
	{
		std::ifstream stream;

		saveManager.getLoadFileStream(&stream);

		if (!stream.fail())
		{
			auto& manager = toolbox.application.find<InputManager>();

			switch (mode)
			{
				case Mode::circuit:
				{
					toolbox.application.grid = Area::readFrom(stream, manager.viewCenter, manager.viewExtend);
					message = "Successfully loaded circuit";

					break;
				}
				case Mode::clipboard:
				{
					manager.find<Clipboard>().readFrom(stream);
					message = "Successfully loaded clipboard";

					break;
				}
			}
		}
		else message = "Failed to open path";
	}

	ImGui::SameLine();

	if (ImGui::Button("Save") && hasName)
	{
		std::ofstream stream;

		saveManager.getSaveFileStream(&stream);

		if (!stream.fail())
		{
			auto& manager = toolbox.application.find<InputManager>();

			switch (mode)
			{
				case Mode::circuit:
				{
					toolbox.application.grid->writeTo(stream, manager.viewCenter, manager.viewExtend);
					message = "Successfully saved circuit";

					break;
				}
				case Mode::clipboard:
				{
					manager.find<Clipboard>().writeTo(stream);
					message = "Successfully saved clipboard";

					break;
				}
			}
		}
		else message = "Failed to open path";
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete") && hasName)
	{
		fs::path path = saveManager.getLastFilePath();

		if (fs::exists(path) && fs::remove(path)) message = "Successfully deleted circuit";
		else message = "Failed to delete path";
	}

	if (!message.empty())
	{
		ImGui::SameLine();
		ImGui::Text(message.c_str());
	}

	if (!hasName)
	{
		ImGui::SameLine();
		ImGui::Text("Missing file name");
	}
}
