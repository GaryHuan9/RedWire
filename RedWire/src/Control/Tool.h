#pragma once

#include "../Type2.h"
#include <memory>

namespace RedWire
{
	struct InputManager;
	struct Grid;

	struct Tool
	{
		Tool(InputManager& manager);

		/// <summary>
		/// Invoked every frame if this tool is selected.
		/// </summary>
		/// <param name="position">The current mouse cursor world position.</param>
		/// <param name="cell">The currently hovered world cell position.</param>
		/// <param name="down">Whether the left mouse button is being pressed right now.</param>
		/// <param name="changed">Whether the left mouse button state changed this frame.</param>
		virtual void update(const Float2& position, const Int2& cell, const bool& down, const bool& changed) = 0;

		/// <summary>
		/// Returns whether this tool should be activated/selected.
		/// </summary>
		virtual bool activationPredicate() = 0;

	protected:

		InputManager& manager;
		std::unique_ptr<Grid>& grid;
	};
}