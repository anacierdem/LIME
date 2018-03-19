//
// Created by anacierdem on 8/18/16.
//
#pragma once

//Dependencies
#include <SFML/Graphics.hpp>
#include "../Misc/VectorExtension.hpp"
#include "../Core/EngineComponent.hpp"
#include "GameObjectManager.hpp"

class TileMap;
class GameObject;

// This is the main component that updates the TileMap, GameObjectManager and runs
// basic editor logic such as scrolling and keyboard shortcuts as well as informing
// player game object with acceleration etc. Also loads the textures.
// This is currently the main file that governs editor/game core logic, without too
// much low level stuff going on. Any routine that may initially fit here must be
// considered to have its own class/file. Would be better to rename as 'EditorController'
class GameRenderer : public NSEngineComponent::EngineComponent {

public:
	GameRenderer();

	void init(sf::RenderWindow* window, const TileMap * map);

	virtual void onBeforeUpdate(V2 pos);
	virtual void onInit();

	int addPlayer(V2 pos, sf::Color col, float scale);
	void setPlayer(int index, V2 pos);
	void removePlayer(int index);

	void injectEvent(sf::Event e) override;
	void receiveMessage(NSEngineComponent::EngineCompenentType source,
						NSEngineComponent::EngineCompenentType,
						NSEngineComponent::MessageData::MessageList) override;
	void update(sf::Time deltaTime) override;
	void resize(sf::Vector2i resolution) override;
	void draw() override;

	sf::View getView() const;

	virtual ~GameRenderer();

private:
	sf::Texture playerTexture;
	sf::Texture objectLayerTexture;
	GameObjectManager gameObjectManager;

	sf::View view;
	sf::Shader shader;

	sf::Vector2i resolution;

	sf::RenderWindow* window;

	bool isDebug = false;

	const TileMap* map;
	std::vector<GameObject*> players;

    GameRenderer(const GameRenderer&);
    GameRenderer& operator=(const GameRenderer&);
};