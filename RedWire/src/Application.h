#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Component.h"

namespace RedWire
{
	struct Grid;
	struct Tool;

	struct Application : sf::RenderWindow
	{
		Application();

		inline const sf::Time& getDeltaTime() { return deltaTime; }
		inline const sf::Time& getTotalTime() { return totalTime; }

		void start();

		void dispatchEvents();

		void update();

		template<typename Type> Type& find();
		std::unique_ptr<Grid> grid;
		std::unordered_map<std::type_index, std::unique_ptr<Component>> components;

	private:
		sf::Clock clock;

		sf::Time deltaTime;
		sf::Time totalTime;
	};

	template<typename Type> inline Type& Application::find()
	{
		return dynamic_cast<Type&>(*components[typeid(Type)]);
	}
}