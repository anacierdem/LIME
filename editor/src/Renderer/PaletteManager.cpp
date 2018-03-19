//
// Created by anacierdem on 7/12/16.
//
#include <cmath>
#include "PaletteManager.hpp"
#include "../Core/EngineComponent.hpp"
#include <Awesomium/JSValue.h>
#include <Awesomium/STLHelpers.h>

using namespace Awesomium;
using namespace NSEngineComponent;

PaletteManager::PaletteManager() {
	myType = PALETTE_MANAGER;
}

void PaletteManager::injectEvent(sf::Event e) {
	if (e.type == sf::Event::MouseButtonPressed) //TODO: draw on first click
	{

	}
	else if (e.type == sf::Event::MouseMoved)
	{
		sf::Uint32 type = 0;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			type = 1;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
			type = 2;
		}

		if (type > 0) {
			// Move this to another component, this is not the final place.
			sendMessage(ENGINE_MANAGER, MessageData::MessageList{
				MakeMessage(std::string("paint")),
				MakeMessage(currentTile),
				MakeMessage(sf::Vector2i(e.mouseMove.x, e.mouseMove.y)),
				MakeMessage(type)
			});
		}
	}
}

void PaletteManager::init(sf::RenderWindow* _window, const TileMap* const _currentMap) {
    //Set render window and map
    window = _window;
}

void PaletteManager::receiveMessage(EngineCompenentType source, EngineCompenentType target, MessageData::MessageList data)
{
	// TODO: move myType checks to a centralized place
	if (target == myType) {
		JSArray jsMessage = GetMessage<JSArray>(data[0]);

		WebString str = jsMessage[0].ToString();
		int len = str.ToUTF8(nullptr, 0);
		char* c_str = new char[len + 1];
		str.ToUTF8(c_str, len + 1);
		std::string command = std::string(c_str);

		if(command == "setTile") {
			_currentTile =  sf::Vector2<sf::Uint8>(jsMessage[1].ToInteger(), jsMessage[2].ToInteger());
		}
	}
}

void PaletteManager::draw() {

}

void PaletteManager::resize(sf::Vector2i resolution)
{
	
}

void PaletteManager::update(sf::Time deltaTime)
{
	
}

