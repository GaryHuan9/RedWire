#pragma once

namespace sf
{
	class Event;
}

namespace RedWire
{
	struct Application;

	struct Component
	{
		Component(Application& application);

		Application& application;

		virtual void update() = 0;
	};
}