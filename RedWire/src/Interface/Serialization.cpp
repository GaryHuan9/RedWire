#include "Toolbox.h"
#include "Serialization.h"
#include "../Application.h"
#include "../Core/Grid.h"
#include "../Control/InputManager.h"
#include "../Control/Clipboard.h"

#include "imgui.h"
#include "imgui_internal.h"
#include <fstream>
#include <string>

#include "../Control/SaveManager.h"

using namespace RedWire;

namespace fs = std::filesystem;

const float Serialization::MESSAGE_DISPLAY_SPAN = 5.f; // second

Serialization::Serialization(Toolbox& toolbox) : Section(toolbox)
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

	static const char* modeNames[] = { "Circuit", "Clipboard" };
	ImGui::SliderInt("Mode", ptr, 0, 1, modeNames[*ptr]);

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
			if (!created) setMessage("Failed to create path");
		}

		ImGui::EndCombo();
	}

	bool hasName = fileName[0] != '\0';

	if (!hasName)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * .5f);
	}

	if (!confirming)
	{
		static const char* fileModeNames[] = { "Load", "Save", "Delete" };

		for (size_t i = 0ull; i < 3; i++)
		{
			if (i != 0) ImGui::SameLine();

			if (ImGui::Button(fileModeNames[i]))
			{
				fileMode = static_cast<FileMode>(i);
				confirming = true;
			}
		}
	}
	else
	{
		if (ImGui::Button("Confirm"))
		{
			confirming = false;

			switch (fileMode)
			{
				case FileMode::load:
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
								setMessage("Successfully loaded circuit");

								break;
							}
							case Mode::clipboard:
							{
								manager.find<Clipboard>().readFrom(stream);
								setMessage("Successfully loaded clipboard");

								break;
							}
						}
					}
					else setMessage("Failed to open path");

					break;
				}
				case FileMode::save:
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
								setMessage("Successfully saved circuit");

								break;
							}
							case Mode::clipboard:
							{
								manager.find<Clipboard>().writeTo(stream);
								setMessage("Successfully saved clipboard");

								break;
							}
						}
					}
					else setMessage("Failed to open path");
					break;
				}
				case FileMode::remove:
				{
					fs::path path = saveManager.getLastFilePath();

					if (fs::exists(path) && fs::remove(path)) setMessage("Successfully deleted circuit");
					else setMessage("Failed to delete path");
					break;
				}
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			confirming = false;
			setMessage("Cancelled");
		}
	}


	if (!hasName)
	{
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}

	bool inTimeSpan = messageTimeSpan < MESSAGE_DISPLAY_SPAN; // I Dunno what to call this boolean
	
	if (!message.empty() && inTimeSpan)
	{
		ImGui::SameLine();
		ImGui::Text(message.c_str());

		messageTimeSpan += toolbox.application.getDeltaTime().asSeconds();
	}

	//except this message can exist forever
	if (!hasName)
	{
		ImGui::SameLine();
		ImGui::Text("Missing file name");
	}
}

void Serialization::setMessage(const char* text)
{
	message = text;
	messageTimeSpan = 0.f;
}
