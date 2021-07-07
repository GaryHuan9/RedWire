#include <memory>
#include "../Type2.h"
#include "Grid.h"
#include "Wire.h"
#include "Port.h"
#include "Join.h"

using namespace RedWire;
using namespace std;

Join::Join()
{

}

uint32_t Join::getColor() const
{
	return getEnabled() ? 0x00FF02FFu : 0x003302FFu;
}

uint8_t Join::getCellId() const
{
	return 6;
}

void Join::refresh(Grid& grid, const Int2& position)
{
	setEnabled(false);

	for (size_t i = 0; i < 2; i++)
	{
		Int2 offset = Int2::edges4[i];

		Wire* wire0 = dynamic_cast<Wire*>(grid.get(position + offset));
		Wire* wire1 = dynamic_cast<Wire*>(grid.get(position - offset));

		if (wire0 == nullptr || wire1 == nullptr) continue;
		setEnabled(true); if (wire0 == wire1) continue;

		//Can create connection between two sides
		wire1->combine(*wire0);

		shared_ptr<Cell> bundle = grid.getPtr(position - offset);

		grid.floodReplace(position + offset, bundle);
		Grid::removeFrom(grid.wires, wire0);
	}
}
