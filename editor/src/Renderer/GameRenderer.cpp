//
// Created by anacierdem on 6/23/16.
//
//TODO: what does renderer do? Should only manage transformations and render stuff, must not init anything!!

#include <random>
#include <iostream>
#include <iterator>

#include "GameRenderer.hpp"
#include "../Misc/Util.hpp"
#include "../globals.hpp"
#include "GameObjectManager.hpp"
#include "../Core/GameObject.hpp"
#include "Collider.hpp"
#include "Controller.hpp"
#include "Player.hpp"
#include "TileMap.hpp"

using namespace std;
using namespace Awesomium;
using namespace NSEngineComponent;

//TODO: need a better constructor & copy contructor
GameRenderer::GameRenderer() : gameObjectManager(&objectLayerTexture)
{
	myType = GAME_RENDERER;
}

//TODO: convert window to const?
//TODO: Another way without passing window around?
void GameRenderer::init(sf::RenderWindow* w, const TileMap* m)
{
	window = w;
	map = m;

	// add the main player to the scene
	addPlayer(V2(500.0f, 500.0f), sf::Color::White, 0.8f);

	// create a view
	view = sf::View();
	view.setCenter(0, 0);

	//TODO: build a resource manager
	//Load out shader
	if (!shader.loadFromFile(BASE_PATH "shader/fragment.frag", sf::Shader::Fragment))
	{
		throw;
	}

	//Load player texture
	if (!playerTexture.loadFromFile(BASE_PATH "textures/isoChar.png"))
	{
		// error...
	}

	if (!objectLayerTexture.loadFromFile(BASE_PATH "textures/iso2.png"))	{
		//error
	}

	isDebug = true; //false: game, true: editor
	onInit();
}

void GameRenderer::resize(sf::Vector2i res){

	//calculate old zoom level
	sf::Vector2i oldResolution = resolution;
	sf::Vector2f oldSize = view.getSize();

	resolution = res;

	//Update size and recover old zoom level
	view.setSize(sf::Vector2f(resolution));

	if (isDebug &&  oldResolution.x > 0.0f) {
		float oldZoom = oldSize.x / oldResolution.x;
		view.zoom(oldZoom);
	}

	//Also update shader resolution
	shader.setUniform("resolution", sf::Vector2f(resolution));
}

void GameRenderer::injectEvent(sf::Event e){
	if (e.type == sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::C)
		{
			isDebug = !isDebug;
			view.setSize(sf::Vector2f(resolution));
		}
	}
	else if (e.type == sf::Event::MouseWheelMoved)
	{
		if (isDebug) {
			//Set view to be able to map cursor position
			window->setView(view);

			//Zoom centered on mouse position
			sf::Vector2f oldPos = view.getCenter();

			sf::Vector2i pixelPos(e.mouseWheel.x, e.mouseWheel.y);
			sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);

			view.setCenter(worldPos);

			float zoomAmount = e.mouseWheel.delta * 0.1f;
			view.zoom(1.0f - zoomAmount);

			view.setCenter(oldPos + (worldPos - oldPos) * zoomAmount);
		}
	}
}

void GameRenderer::update(sf::Time deltaTime){

	Player* player = static_cast<Player*>(*players.begin());
	onBeforeUpdate(player->getPosition());

	//move the view
	if (isDebug) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			view.move(-view.getSize().x * deltaTime.asSeconds(), 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			view.move(view.getSize().x * deltaTime.asSeconds(), 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			view.move(0, -view.getSize().x * deltaTime.asSeconds());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			view.move(0, view.getSize().x * deltaTime.asSeconds());
		}
	}
	else { //Move player
		//TODO: covert accel to force
		V2 totalAccel = V2(0.0f, 0.0f);
		float baseAccel = 5000.0f * deltaTime.asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			totalAccel.x -= baseAccel;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			totalAccel.x += baseAccel;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			totalAccel.y -= baseAccel;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			totalAccel.y += baseAccel;
		}

		if (totalAccel.length() > 0.001) {
			player->getComponent<Controller>()->accelerate(Util::isoToTwoD(totalAccel));
		}
		else {
			player->getComponent<Controller>()->stopMovement();
		}
	}

	//All calculations below in screen space
	V2 playerPosition = Util::twoDToIso(player->getPosition());

	//Set view to be able to find the bounds TODO: cannot we include inside object manager?
	window->setView(view);
	// Game renderer is not the best place to do this.
	gameObjectManager.forEachVisibleItem(window, [this, playerPosition, player](GameObject * currObject){
		if (gameObjectManager.isVisible(currObject)) {
			Collider* collider = currObject->getComponent<Collider>();
            if (collider->check(playerPosition)) {
                V2 currentNorm = collider->findNormal(playerPosition);

                //TODO: Also prevent entering the region
                V2 currentVelocity = Util::twoDToIso(player->getComponent<Controller>()->getVelocity());
                float dotProduct = (currentNorm * currentVelocity);
                if (dotProduct > 0.0f)
                    player->getComponent<Controller>()->setVelocity(Util::isoToTwoD((currentVelocity - currentNorm * dotProduct)));
                return true;
            }
		}
		return false;
	} );

	if (!isDebug)
		view.setCenter(Util::twoDToIso(player->getPosition()));
	
	gameObjectManager.addObjects(players);
	gameObjectManager.update(deltaTime);
}

void GameRenderer::draw(){

	//set shader parameters
	shader.setUniform("texture", sf::Shader::CurrentTexture);

	//Update shader params
	sf::Vector2f viewSize= view.getSize();

	float scaleAdjust = resolution.x / viewSize.x;
	float range = 1000.0f * scaleAdjust;
	shader.setUniform("range", range * range);

	window->setView(view);
	if (isDebug) {
		map->draw(window, nullptr);
	}
	else {
		map->draw(window, &shader);
	}

	if (isDebug) {
		gameObjectManager.draw(window, nullptr);
	}
	else {
		gameObjectManager.draw(window, &shader);
	}
}

void GameRenderer::receiveMessage(EngineCompenentType source, EngineCompenentType target, MessageData::MessageList data)
{
	// TODO: move myType checks to a centralized place
	if (target == myType) {

	}
}

sf::Int32 GameRenderer::addPlayer(V2 pos, sf::Color col, float scale) {
	Player* tmpPlayer = new Player(&playerTexture);
	tmpPlayer->setPosition(pos);
	tmpPlayer->setScale(scale);
	tmpPlayer->setType(col);

	players.push_back(tmpPlayer);
	return static_cast<sf::Int32>(players.size()) - 1;
}

void GameRenderer::setPlayer(int index, V2 pos) {
	static_cast<Player*>(players[index])->setPosition(pos);
}

void GameRenderer::removePlayer(int index) {
	delete *(std::next(players.begin(), index));
	players.erase(std::next(players.begin(), index));
}

void GameRenderer::onBeforeUpdate(V2 pos)
{
	
}

void GameRenderer::onInit()
{
	
}

sf::View GameRenderer::getView() const
{
	return view;
}

GameRenderer::~GameRenderer() {
	for (std::vector<GameObject*>::iterator it = players.begin(); it != players.end(); ++it) {
		delete *it;
	}

	players.clear();
}