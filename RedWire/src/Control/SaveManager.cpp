#include "SaveManager.h"

#include "../Application.h"
#include "InputManager.h"
#include "../Core/Grid.h"

using namespace RedWire;

SaveManager::SaveManager(Application& application) : Component(application), lastFileName()
{}

//do nothing in update
void SaveManager::update() {}

void SaveManager::setLast(const std::string& target)
{
	lastFileName.fill('\0');
	std::copy(target.begin(), target.end(), lastFileName.begin());
}

void SaveManager::getLoadFileStream(std::ifstream* stream) const
{
	(*stream) = std::ifstream(getLastFilePath(), std::ifstream::binary);
}

void SaveManager::getSaveFileStream(std::ofstream* stream) const
{
	(*stream) = std::ofstream(getLastFilePath(), std::ostream::trunc | std::ostream::binary);
}

bool SaveManager::saveCircuit() const
{
	std::ofstream stream;
	getSaveFileStream(&stream);

	if (stream.fail()) return false;
	//else
	auto& manager = application.find<InputManager>();

	application.grid->writeTo(stream, manager.viewCenter, manager.viewExtend);

	return true;
}
