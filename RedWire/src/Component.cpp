#include "SFML/Window.hpp"

#include "Application.h"
#include "Component.h"

using namespace RedWire;

Component::Component(Application& application) : application(application)
{}
