#include "CoreEngine.hpp"
#include <queue>
#include <Awesomium/STLHelpers.h>
#include "../UI/CustomSurfaceFactory.hpp"
#include "../Misc/Util.hpp"
#include "../UI/CustomSurface.hpp"
#include "CustomHandler.hpp"
#include "../UI/CustomListener.hpp"

using namespace std;
using namespace Awesomium;

CoreEngine::CoreEngine()
{
	isRunning = false;
    sf::Uint32 style = sf::Style::Default;
	sf::VideoMode videoMode;
    if (isFullscreen) {
        style = 1 << 3;
        videoMode = sf::VideoMode::getFullscreenModes()[0];
    }else {
        videoMode = sf::VideoMode::getDesktopMode();
    }

	resolution = sf::Vector2i(videoMode.width, videoMode.height);
//	resolution = sf::Vector2i(5120, 2880);

	//Init SFML ------------------------------------------------------------------------
	window = new sf::RenderWindow (videoMode, "Desert", style);
//  TODO: Do release in XCode for the correct results
}

void CoreEngine::start() {
	isRunning = true;

	sf::Clock clock;
	sf::Time lastUpdate = clock.restart();

	//Averaging window for FPS
	queue<float> FPShistory;
	int fpsWindow = 60;
	for (int i = 0; i < fpsWindow; i++) {
		FPShistory.push(0.0f);
	}
	float sumFPS;
	float lastSum = 0.0f;

	sf::Int64 webViewTime = 0;

	//Load font
	sf::Font font;
	if (!font.loadFromFile(BASE_PATH "fonts/Inconsolata-Regular.ttf"))
	{
		// error...
	}

	//Splash screen
	window->clear(sf::Color::White);
	sf::Text text;
	text.setFont(font);
	text.setString("Loading...");
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	text.setPosition(resolution.x / 2, resolution.y / 2);
	window->draw(text);
	window->display();
	//End splash

	window->setKeyRepeatEnabled(false);
	window->setVerticalSyncEnabled(VERTICAL_SYNC);

	//On screen display view
	OSDView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(resolution));
	OSDView.setCenter(sf::Vector2f(resolution / 2));
	OSDView.setSize(sf::Vector2f(resolution));

	WebConfig myConfig = WebConfig();
	myConfig.remote_debugging_host = WSLit("127.0.0.1");
	myConfig.remote_debugging_port = 8001;

	webCore = WebCore::Initialize(myConfig);
	currentSession = webCore->CreateWebSession(WSLit(""), WebPreferences());

	//DataSource* data_source = new CustomSource();
	//currentSession->AddDataSource(WSLit("Desert"), data_source);

	CustomSurfaceFactory myFactory;
	webCore->set_surface_factory(&myFactory);

	// Create a new WebView
	createWebView();
	JSValue result = mainWebView->CreateGlobalJavascriptObject(WSLit("nativeObject"));
	JSObject interfaceToJS = result.ToObject();

	//Insert all available modules here
	interfaceToJS.SetCustomMethod(WSLit("engineManager"), true);
	interfaceToJS.SetCustomMethod(WSLit("gameRenderer"), true);
	interfaceToJS.SetCustomMethod(WSLit("miniMapManager"), true);
	interfaceToJS.SetCustomMethod(WSLit("paletteManager"), true);

    using namespace std::placeholders;
	CustomHandler handler(std::bind(&CoreEngine::broadcastMessage, this, _1, _2, _3));
    mainWebView->set_js_method_handler(static_cast<JSMethodHandler*>(&handler));

	CustomListener listener(window);
	mainWebView->set_view_listener(&listener);

	onInit();
	resize();

	sf::Sprite webSprite;
	webSprite.setPosition(0, 0);

	while (window->isOpen() && isRunning)
	{
		deltaTime = clock.restart() - lastUpdate;
		onBeforeUpdate();

		//Calculate FPS
		float currentFPS = 1.0f / deltaTime.asSeconds();
		FPShistory.pop();
		FPShistory.push(currentFPS);
		sumFPS = currentFPS - FPShistory.front() + lastSum;
		lastSum = sumFPS;

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window->close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					stop();
				}else if (event.key.code == sf::Keyboard::F5)
				{
					//TODO do this only in debug mode
					mainWebView->Reload(true);
				}
				Util::handleKeyEvent(mainWebView, event);
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				Util::handleKeyEvent(mainWebView, event);
			}
			else if (event.type == sf::Event::TextEntered)
			{
				Util::handleKeyEvent(mainWebView, event);
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				mainWebView->InjectMouseWheel(event.mouseWheel.delta * 10, 0);
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				//Send view to be able to transform coordinates correctly
				sf::Vector2f newPos = window->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), OSDView);
				mainWebView->InjectMouseMove(static_cast<sf::Int32>(newPos.x),
					static_cast<sf::Int32>(newPos.y));
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				mainWebView->InjectMouseDown(kMouseButton_Left);
			}
			else if (event.type == sf::Event::MouseButtonReleased)
			{
				mainWebView->InjectMouseUp(kMouseButton_Left);
			}
			else if (event.type == sf::Event::Resized)
			{
				resize();
			}
			onEvent(event);
		}

		//Clear and render
		window->clear(sf::Color::Black);

		onRender();

		//switch to overlay layer
		window->setView(OSDView);
		onDrawGUI();

		//Render webview
		sf::Int64 preWebClock = clock.getElapsedTime().asMicroseconds();

		//Reload web view if crashed
		if (mainWebView->IsCrashed()) {
			createWebView();
			loadWebView();
		}

		onBeforeWebViewUpdate();

		//Update and draw web view
		webCore->Update();
		CustomSurface* surface = static_cast<CustomSurface*>(mainWebView->surface());
		if (surface) {
			webSprite.setTexture(surface->renderBuffer.getTexture());
			window->draw(webSprite);
		}

		webViewTime = clock.getElapsedTime().asMicroseconds() - preWebClock;

		//Draw Infoline
		text.setFont(font);
		text.setString(to_string(sumFPS / fpsWindow) +
			" web view rendered in: " + to_string(webViewTime / 1000.0f) + " ms");
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::Black);
		text.setPosition(0, 0);
		window->draw(text);

		text.setPosition(1, 1);
		text.setFillColor(sf::Color::White);
		window->draw(text);

		//present
		window->display();
	}

	mainWebView->Destroy();
	WebCore::Shutdown();
}

void CoreEngine::resize()
{
	resolution = sf::Vector2i(window->getSize());

	//Resize overlay layer
	OSDView.setCenter(sf::Vector2f(resolution / 2));
	OSDView.setSize(sf::Vector2f(resolution));

	onResize(resolution);

	mainWebView->Resize(resolution.x, resolution.y);
}
void CoreEngine::stop()
{
	isRunning = false;
}

void CoreEngine::createWebView()
{
	sf::Vector2f size(resolution);
	size *= 4.0f;
	mainWebView = webCore->CreateWebView(size.x, size.y, currentSession, kWebViewType_Offscreen);
	mainWebView->Resize(size.x, size.y);
	mainWebView->SetTransparent(true);
}

void CoreEngine::sendWebViewMessage(JSArray args) {
	JSValue window = mainWebView->ExecuteJavascriptWithResult(
			WSLit("window"), WSLit(""));

	if (window.IsObject()) {
		window.ToObject().Invoke(WSLit("handleNativeMessage"), args);
	}
}

void CoreEngine::setWebViewURL(string url)
{
	webViewURL = url;
}

void CoreEngine::loadWebView()
{
	string tmpStr = "file:///" + BASE_PATH webViewURL;
	mainWebView->LoadURL(WebURL(WebString::CreateFromUTF8(tmpStr.c_str(), tmpStr.length())));
	mainWebView->Focus();
}

CoreEngine::~CoreEngine()
{
	window->close();
	delete window;
}
