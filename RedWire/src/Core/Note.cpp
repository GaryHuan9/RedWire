#include "Note.h"

using namespace RedWire;

uint32_t RedWire::Note::getColor() const
{
	return 0xFF100C0Cu;
	//return 0xFFfbf236u;
}

uint8_t RedWire::Note::getCellId() const
{
	return 7u;
}
