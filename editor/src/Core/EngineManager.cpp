//
// Created by Ali Naci  Erdem on 29/08/16.
//

#include <fstream>
#include "EngineManager.hpp"
#include "../Misc/SaveMethods.hpp"
#include "../Renderer/TileMap.hpp"

using namespace AlEngine;
using namespace Awesomium;

EngineManager::EngineManager()
{
	map = new TileMap(1024);
}

void EngineManager::broadcastMessage(EngineCompenentType source, EngineCompenentType target, MessageData::MessageList data) {
	if (target == ENGINE_MANAGER)
	{
		std::string command;
		if (source == WEB_VIEW)
		{
			//TODO: GetMEssageFirstItem as string?
			WebString str = GetMessage<JSArray>(data[0])[0].ToString();
			int len = str.ToUTF8(nullptr, 0);
			char* c_str = new char[len + 1];
			str.ToUTF8(c_str, len + 1);
			command = std::string(c_str);
		} else
		{
			command = GetMessage<std::string>(data[0]);
		}

		//TODO: Move save-load-reset-paint to e.g map manager
		if (command == "saveMap")
		{
            std::ofstream file(BASE_PATH"other/tileMap.dmap", std::ios::out | std::ios::binary);

            Serialization::save(file, map);

            file.close();
		}
		else if (command == "loadMap")
		{
            std::ifstream file(BASE_PATH"other/tileMap.dmap", std::ios::in | std::ios::binary);
            file.seekg(0, std::ios::beg);

            Serialization::load<TileMap>(file, map);

            file.close();
		}
		else if (command == "resetMap")
		{
			map->reset(false);
		}
		else if (command == "paint")
		{
			//Set view to be able to transform coordinates correctly
			//TODO: move this (map pixel to coords) to renderer?
			window->setView(renderer.getView());
			sf::Vector2f worldPos = window->mapPixelToCoords(GetMessage<sf::Vector2i>(data[2]));

			// There is also a setTile method
			map->setTileBrush(worldPos, paletteManager.currentTile);
		}else if (command == "mouseCaptured")
        {
            mouseCapturedByWebView = true;
        }else if (command == "mouseReleased")
        {
            mouseCapturedByWebView = false;
        }
	}
	else if (target == WEB_VIEW)
	{
		JSArray tmp = GetMessage<JSArray>(data[0]);

		WebString str = tmp[0].ToString();
		int len = str.ToUTF8(nullptr, 0);
		char* c_str = new char[len + 1];
		str.ToUTF8(c_str, len + 1);

		sendWebViewMessage(tmp);
	}
	else {
		renderer.receiveMessage(source, target, data);
		paletteManager.receiveMessage(source, target, data);
		miniMapManager.receiveMessage(source, target, data);
	}
}

void EngineManager::onInit()
{
    //TODO: wait for webview loaded event somewhere
    setWebViewURL("html/index.html");
    loadWebView();

    using namespace std::placeholders;
	renderer.setMessageInterface(std::bind(&EngineManager::broadcastMessage, this, _1, _2, _3));
	paletteManager.setMessageInterface(std::bind(&EngineManager::broadcastMessage, this, _1, _2, _3));
	miniMapManager.setMessageInterface(std::bind(&EngineManager::broadcastMessage, this, _1, _2, _3));

	map->create(BASE_PATH "textures/iso2.png", false);

	//TODO: initializing pallette manager before maps should be possible
	//Move these to web view
	paletteManager.init(window, map);
	miniMapManager.init(window, map);
	renderer.init(window, map);
}

void EngineManager::onBeforeUpdate()
{
    renderer.update(deltaTime);
}

void EngineManager::onEvent(sf::Event e)
{
	//For testing purposes only
	/*if (e.type == sf::Event::TextEntered)
	{
		wchar16* chr = reinterpret_cast<wchar16*>(&e.text.unicode);
		JSArray args = JSArray();
		args.Push(WebString(chr));
		sendWebViewMessage(args); 
	}*/

	// Only inject SFML events if mouse is not being captured purposefully by webview
    if(!mouseCapturedByWebView) {
        paletteManager.injectEvent(e);
        miniMapManager.injectEvent(e);
        renderer.injectEvent(e);
    }
}

void EngineManager::onResize(sf::Vector2i res)
{
	resolution = res;
	renderer.resize(resolution);
	miniMapManager.resize(resolution);
}

void EngineManager::onRender()
{
	renderer.draw();
}

void EngineManager::onDrawGUI()
{
	miniMapManager.draw();
	paletteManager.draw();
}

void EngineManager::onBeforeWebViewUpdate()
{

}

EngineManager::~EngineManager()
{
	delete map;
}