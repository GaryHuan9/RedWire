#pragma once



namespace RedWire
{

	struct Toolbox;

	struct Section
	{
		Section(Toolbox& toolbox);

		virtual void show() = 0;

		Toolbox& toolbox;
	};
}