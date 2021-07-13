#pragma once

#include "SFML/System.hpp"
#include "../Component.h"

namespace RedWire
{
	struct TickManager : Component
	{
		TickManager(Application& application);

		virtual void update() override;

		void updateGrid();

		enum class Mode
		{
			perSecond,
			perFrame,
			manual
		};

		Mode mode = Mode::perSecond;
		size_t magnitude = 1;

	private:
		void updateSingle();

		bool manualPressed{};
		sf::Time timeRemain{};
	};
}