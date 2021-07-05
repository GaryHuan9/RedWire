#pragma once

namespace RedWire
{
	struct Port;

	struct Join : Port
	{
		Join();

		uint32_t getColor() const override;
	};
}