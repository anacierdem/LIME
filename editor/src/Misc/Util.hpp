//
// Created by Ali Naci  Erdem on 24/08/16.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <Awesomium/WebCore.h>

namespace Util {

	//Coordinate transformations
	inline sf::Vector2f twoDToIso(sf::Vector2f point) {
		return(sf::Vector2f((point.x - point.y) / 2.0f, (point.x + point.y) / 4.0f));
	}

	inline sf::Vector2f isoToTwoD(sf::Vector2f point) {
		return(sf::Vector2f((2.0f * point.y + point.x), (2.0f * point.y - point.x)));
	}

	//simplified coordinate transformations
	inline bool compareDepth(sf::Vector2f a, sf::Vector2f b) {
		return a.x + a.y < b.x + b.y;
	}

    void handleKeyEvent(Awesomium::WebView* webView, const sf::Event e);

	static int getWebKeyFromSFMLKey(sf::Keyboard::Key key);
};
