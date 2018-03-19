#pragma once

//Dependencies
#include <SFML/Graphics.hpp>
#include "../Misc/VectorExtension.hpp"
#include "../Core/GameObject.hpp"

// This is a game object that displays a player. Originally intended as
// the user-controlled player but serves as well as a debugging object.
class Player : public GameObject{

	sf::Texture const * atlas;

	//Temp components
	Renderer* renderer;
	Controller* controller;

public:
	Player(const sf::Texture* const atlas);
	Player(const Player& in);
	Player();
	~Player();

    void update(sf::Time) override;

	void setType(sf::Color col);
};