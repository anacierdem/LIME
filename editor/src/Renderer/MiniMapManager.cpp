//
// Created by anacierdem on 7/13/16.
//

#include "MiniMapManager.hpp"
#include "TileMap.hpp"
#include <Awesomium/JSValue.h>
#include <Awesomium/STLHelpers.h>
#include <string>

MiniMapManager::MiniMapManager() {
	myType = MINI_MAP_MANAGER;
}

void MiniMapManager::init(sf::RenderWindow* w, TileMap* map) {
    window = w;

    miniMapDisplay.setOrigin(0, 0);
    miniMapDisplay.setTexture(map->miniMapTexture);
    size = sf::Vector2f(map->miniMapTexture.getSize()) * 0.25f;
    miniMapDisplay.setScale(0.25f, 0.25f);
}

void MiniMapManager::draw() {
    miniMapDisplay.setPosition(resolution.x - size.x, 0);
    window->draw(miniMapDisplay);
}

void MiniMapManager::receiveMessage(EngineCompenentType source, EngineCompenentType target, MessageData::MessageList data)
{
	// TODO: move myType checks to a centralized place
	if (target == myType) {

	}
}

void MiniMapManager::resize(sf::Vector2i res)
{
	resolution = res;
}

void MiniMapManager::update(sf::Time deltaTime)
{

}

void MiniMapManager::injectEvent(sf::Event e) {
	if (e.type == sf::Event::KeyPressed)
	{
		// Move these to another component, this is not the final place.
		if (e.key.code == sf::Keyboard::K)
		{
			sendMessage(ENGINE_MANAGER, MessageData::MessageList {
				MakeMessage(std::string("saveMap"))
			});
		}
		else if (e.key.code == sf::Keyboard::L){
			sendMessage(ENGINE_MANAGER, MessageData::MessageList{
				MakeMessage(std::string("loadMap"))
			});
		}
		else if (e.key.code == sf::Keyboard::N)
		{
			sendMessage(ENGINE_MANAGER, MessageData::MessageList{
				MakeMessage(std::string("resetMap"))
			});
		}
		else if (e.key.code == sf::Keyboard::Q)
		{
			// Example webview message
			Awesomium::JSArray testMsg = Awesomium::JSArray();
			testMsg.Push(Awesomium::JSValue(Awesomium::WSLit("Action")));
			testMsg.Push(Awesomium::JSValue(Awesomium::WSLit("Params")));

			sendMessage(WEB_VIEW, MessageData::MessageList{
				MakeMessage(testMsg)
			});
		}
	}
}
