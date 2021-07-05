#include "Port.h"
#include "Join.h"

using namespace RedWire;

Join::Join()
{

}

uint32_t Join::getColor() const
{
	return getEnabled() ? 0x00FF02FFu : 0x003302FFu;
}
