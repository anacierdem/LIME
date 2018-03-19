
//Dependencies

#include "Controller.hpp"

Controller::Controller() {

}

void Controller::accelerate(V2 amount) {
	isMoving = true;
	velocity += amount;
}

void Controller::stopMovement() {
	isMoving = false;
}

V2 Controller::getVelocity() const{
	return velocity;
}

void Controller::setVelocity(V2 vel){
	velocity = vel;
}

void Controller::update(sf::Time deltaTime){
    //If not moving - slow down and stop
    if (!isMoving) {
        velocity -= drag*velocity*deltaTime.asSeconds();
    }

    //Limit velocity
    float velocityMagnitude = velocity.length();
    if (velocityMagnitude >= maxVelocity) {
        velocity = velocity.normalized() * maxVelocity;
    }

    //Update position
    owner->position += velocity * deltaTime.asSeconds();
}