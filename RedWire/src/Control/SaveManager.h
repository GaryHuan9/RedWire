#pragma once

#include "../Component.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <array>

namespace fs = std::filesystem;

namespace RedWire
{
	struct Application;

	struct SaveManager : Component
	{
		SaveManager(Application& application);

		void update() override;

		void setLast(const std::string& lastName);

		inline std::array<char, 100ull>& getLast() { return lastFileName; }

		inline const std::array<char, 100ull>& getLast() const { return lastFileName; }

		inline fs::directory_iterator getSavesIterator() const { return fs::directory_iterator(savePath); }

		inline fs::path getLastFilePath() const { return savePath / lastFileName.data(); }

		void getLoadFileStream(std::ifstream* stream) const;

		void getSaveFileStream(std::ofstream* stream) const;

		bool saveCircuit() const;

		const fs::path savePath = "saves";

	private:
		//last opened file name
		std::array<char, 100ull> lastFileName;
	};
}


