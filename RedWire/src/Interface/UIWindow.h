#pragma once

#include "../Component.h"

namespace RedWire
{
	struct Application;

	/*
	* The UI window will be used for UI Manager, it will manage all the windows, so we can display or hide it
	*/
	struct UIWindow : Component
	{
		UIWindow(Application& application);

		inline bool isActive() { return active; }

		inline void setActive(const bool& value) { active = value; }

		inline void toggleActive() { active = !active; }

		void update() override;

	protected:
		virtual void showUI() = 0;

	private:
		bool active = false;
	};
}


