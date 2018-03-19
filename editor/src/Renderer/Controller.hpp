#pragma once

//Dependencies
#include "../Core/GameObjectComponent.hpp"

// A simple component to provide basic physics controls
class Controller : public GameObjectComponent{

	bool isMoving;

	float drag = 20.0f;
	float maxVelocity = 256.0f;

    //TODO: move velocity to rigidbody
	V2 velocity;

public:
	Controller();

	void update(sf::Time);

	void accelerate(V2 acc);
	void stopMovement();

	V2 getVelocity() const;
	void setVelocity(V2 vel);
};

