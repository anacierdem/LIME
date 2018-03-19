#pragma once

#include <Awesomium/WebCore.h>
#include "../globals.hpp"
#include "EngineComponent.hpp"

// Initializes Awesomium and keeps track of general engine state.
class CoreEngine
{
public:
	CoreEngine();

	void start();
	void stop();
	void loadWebView();
	void setWebViewURL(std::string url);
	void sendWebViewMessage(Awesomium::JSArray args);

	virtual void broadcastMessage(NSEngineComponent::EngineCompenentType source,
								  NSEngineComponent::EngineCompenentType target,
								  NSEngineComponent::MessageData::MessageList data) = 0;

	virtual void onInit() = 0;
	virtual void onBeforeUpdate() = 0;
	virtual void onEvent(sf::Event e) = 0;
	virtual void onResize(sf::Vector2i resolution) = 0;
	virtual void onRender() = 0;
	virtual void onDrawGUI() = 0;
	virtual void onBeforeWebViewUpdate() = 0;

	virtual ~CoreEngine();

protected:
	sf::RenderWindow* window;
	sf::Time deltaTime;
	sf::Vector2i resolution;

private:
	void createWebView();

	std::string webViewURL;
	Awesomium::WebCore* webCore;
	Awesomium::WebSession* currentSession;
	Awesomium::WebView* mainWebView;
	bool isRunning;

	CoreEngine(const CoreEngine&);
	CoreEngine& operator=(const CoreEngine&);

	sf::View OSDView;
	void resize();

	bool isFullscreen = IS_FULLSCREEN;
};
