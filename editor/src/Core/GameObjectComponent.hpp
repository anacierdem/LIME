#pragma once

#include "GameObject.hpp"

class GameObject;

class GameObjectComponent {


public:
	GameObjectComponent();
	GameObject* owner;

	virtual void update(sf::Time) {};
};
