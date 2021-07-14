#pragma once

namespace RedWire
{
	//Not sure if this is the best solution, I tried
	struct HotkeyAction
	{
		bool checkActivation();

		virtual void doAction() = 0;

	protected:
		virtual bool activationPredicate() = 0;

	private:
		bool initialActivate = false;
	};
}


