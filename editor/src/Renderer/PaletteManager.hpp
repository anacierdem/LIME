//
// Created by anacierdem on 7/12/16.
//

#pragma once

//Dependencies
#include "../Core/EngineComponent.hpp"

class TileMap;

// Keeps track of the selected tile by listening to the webview
class PaletteManager : public NSEngineComponent::EngineComponent {

    sf::RenderWindow * window;
    sf::Vector2<sf::Uint8> _currentTile;

public:
    const sf::Vector2<sf::Uint8> &currentTile = _currentTile;

	void injectEvent(sf::Event e) override;

	void receiveMessage(NSEngineComponent::EngineCompenentType,
						NSEngineComponent::EngineCompenentType,
						NSEngineComponent::MessageData::MessageList) override;
	void update(sf::Time deltaTime) override;
	void resize(sf::Vector2i resolution) override;
	void draw() override;

    PaletteManager();

    void init(sf::RenderWindow* _window, const TileMap* const _currentMap);
};