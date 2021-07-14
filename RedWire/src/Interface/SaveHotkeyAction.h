#pragma once
#include "HotkeyAction.h"

namespace RedWire
{
	struct Application;

	struct SaveHotkeyAction : HotkeyAction
	{
		SaveHotkeyAction(Application& application);

		void doAction() override;

	protected:
		virtual bool activationPredicate() override;

	private:
		Application& application;
	};
}


