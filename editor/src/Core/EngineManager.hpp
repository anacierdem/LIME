//
// Created by Ali Naci  Erdem on 29/08/16.
//

#ifndef EDITOR_MANAGER_HPP
#define EDITOR_MANAGER_HPP

#include "CoreEngine.hpp"
#include "../Application/Application.hpp"

#include "../Renderer/PaletteManager.hpp"
#include "../Renderer/MiniMapManager.hpp"

namespace AlEngine{

	// Manages engineComponents. Distributes messages among webview
	// and engine components. Currently also handling load/save messages, paints etc.
	// TODO: These operations should be moved into their respective components as this
	// file gets larger.
    class EngineManager final: public CoreEngine {
    public:
		EngineManager();
		~EngineManager();

		void onInit() final;
		void onBeforeUpdate() final;
		void onEvent(sf::Event e) final;
		void onResize(sf::Vector2i resolution) final;
		void onRender() final;
		void onDrawGUI() final;
		void onBeforeWebViewUpdate() final;

		void broadcastMessage(EngineCompenentType source, EngineCompenentType target, MessageData::MessageList data) final;

    private:
		Application renderer;

		PaletteManager paletteManager;
		MiniMapManager miniMapManager;

		TileMap* map;

		bool mouseCapturedByWebView = false;
    };
}


#endif //EDITOR_MANAGER_HPP
