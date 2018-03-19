
//Dependencies

#include "../Misc/Util.hpp"
#include "Renderer.hpp"

Renderer::Renderer() {

}

Renderer::Renderer(const Renderer& in) {

}

Renderer::Renderer(V2 offset, sf::Rect<int> rect) {
    setOrigin(offset);
    setTextureRect(rect);
}

void Renderer::draw(sf::RenderWindow* window) {
	setPosition(Util::twoDToIso(owner->position));
	setScale(owner->scale, owner->scale);
    window->draw(*static_cast<sf::Sprite*>(this));
}