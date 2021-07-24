#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include "../Component.h"
#include "../Interface/Window/UIWindow.h"

namespace RedWire
{
	struct Application;

	struct UIManager : Component
	{
		UIManager(Application& application);

		void update() override;

		template<typename Type> Type& find();

	private:
		std::unordered_map<std::type_index, std::unique_ptr<UIWindow>> windows;
	};

	template<typename Type> inline Type& UIManager::find()
	{
		return dynamic_cast<Type&>(*windows[typeid(Type)]);
	}
}

