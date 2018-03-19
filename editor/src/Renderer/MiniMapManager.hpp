//
// Created by anacierdem on 7/13/16.
//

#pragma once

//dependencies
#include <SFML/Graphics.hpp>
#include "../Core/EngineComponent.hpp"

using namespace NSEngineComponent;

class TileMap;

// Manages rendering of the mini map on top right. Also currently
// handling some map related key events
class MiniMapManager : public EngineComponent {
public:
	MiniMapManager();
	void init(sf::RenderWindow* window, TileMap* map);

	void injectEvent(sf::Event e) override;

	void receiveMessage(EngineCompenentType, EngineCompenentType, MessageData::MessageList) override;
	void update(sf::Time deltaTime) override;
	void resize(sf::Vector2i resolution) override;
	void draw() override;

private:
	sf::Vector2i resolution;
	sf::Sprite miniMapDisplay;
	sf::RenderWindow* window;
	sf::Vector2f size;
};
