#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.hpp"
#include "Renderer.hpp"
#include "Controller.hpp"

Player::Player(const Player& in) {
	position = in.position;
	atlas = in.atlas;
	scale = in.scale;
	renderer = new Renderer(*in.renderer);
	controller = new Controller(*in.controller);
	addComponent(renderer);
    addComponent(controller);
}

Player::Player() {
	position = V2(0.0f, 0.0f);
	atlas = nullptr;
	scale = 0.8f;
	renderer = new Renderer();
	controller = new Controller();
	addComponent(renderer);
	addComponent(controller);
}

Player::Player(const sf::Texture* const _atlas)  {
	position = V2(5000.0f, 5000.0f);
	atlas = _atlas;
	scale = 0.8f;
	renderer = new Renderer();
	controller = new Controller();
	addComponent(renderer);
	addComponent(controller);
}

void Player::update(sf::Time deltaTime){
	Renderer* renderer = getComponent<Renderer>();
	//Test adjustments -  TODO: move to methods
	renderer->setOrigin(38, 135);
	renderer->setTexture(*atlas);

    //Find angle from velocity vector
    Controller* controller = getComponent<Controller>();
    float angle = static_cast<float>(atan2(controller->getVelocity().y, controller->getVelocity().x)) * 180.0f / static_cast<float>(M_PI);

    //Sprite sheet mapping - TODO: convert into methods
	sf::Int16 offset = sf::Int16(round((angle - 45) / 45));
	offset = offset < 0 ? sf::Int16(8) + offset : offset;
	renderer->setTextureRect(sf::IntRect(offset * 90, 0, 90, 428));

	//Also update base class (update all compoenents)
	GameObject::update(deltaTime);
}

void Player::setType(sf::Color col){

	Renderer* renderer = getComponent<Renderer>();

	//Test adjustments -  TODO: move to methods
	renderer->setColor(col);
}

Player::~Player()
{

}
