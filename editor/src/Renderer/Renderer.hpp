#pragma once

//Dependencies
#include "../Misc/VectorExtension.hpp"
#include <SFML/Graphics.hpp>
#include "../Core/GameObjectComponent.hpp"

// A GameObjectComponent capable of renderinf using SFML
class Renderer : public GameObjectComponent, public sf::Sprite{
	Renderer& operator=(const Renderer&);
	
public:
	Renderer();
	Renderer(const Renderer&);
	Renderer(V2 offset, sf::Rect<int> rect);

	void draw(sf::RenderWindow* window);
};

