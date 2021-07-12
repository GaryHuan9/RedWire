#pragma once

#include <SFML/Window.hpp>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>
#include <array>
#include "Tool.h"
#include "../Component.h"
#include "../Type2.h"

namespace RedWire
{
	struct Application;

	struct InputManager : Component
	{
		InputManager(Application& application);

		void onEventPoll(const sf::Event& event);

		static bool isPressed(const sf::Keyboard::Key& key);
		static bool isPressed(const sf::Mouse::Button& button);

		virtual void update() override;

		Tool* getCurrentTool() const;
		void setCurrentTool(Tool* tool);

		Float2 viewCenter;
		float  viewExtend;

		template<typename Type> Type& find();

		std::unordered_map<std::type_index, std::unique_ptr<Tool>> tools;

	private:
		Float2 getMousePosition(); //Returns the world position of the mouse

		bool leftMouse{};
		bool middleMouse{};

		Tool* currentTool{ 0 };
		Float2 pressedPosition;
	};

	template<typename Type> inline Type& InputManager::find()
	{
		return dynamic_cast<Type&>(*tools[typeid(Type)]);
	}
}

