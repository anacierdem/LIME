#include "CustomSurface.hpp"
#include "../globals.hpp"

CustomSurface::CustomSurface(sf::Uint16 w, sf::Uint16 h)
{
	width = w;
	height = h;

	textureToUpdate.create(width, height);
	renderBuffer.create(width, height);
	renderBuffer2.create(width, height);

    //R-G inversion shader
	if (!shader.loadFromFile(BASE_PATH "shader/invertRG.frag", sf::Shader::Fragment))
	{
		throw;
	}

    shader.setUniform("texture", sf::Shader::CurrentTexture);
}

void CustomSurface::Paint(unsigned char* src_buffer,
	int src_row_span,
	const Awesomium::Rect& src_rect,
	const Awesomium::Rect& dest_rect)
{
	//Render unchanged parts
	tmpSprite.setTexture(renderBuffer.getTexture());

    //Only use texture at hand (do not blend, discard destination)
	sf::BlendMode blend;
	blend.alphaDstFactor = sf::BlendMode::Factor::Zero;
	blend.alphaSrcFactor = sf::BlendMode::Factor::One;

	blend.colorSrcFactor = sf::BlendMode::Factor::One;
	blend.colorDstFactor = sf::BlendMode::Factor::Zero;

	blend.alphaEquation = sf::BlendMode::Equation::Add;
	blend.colorEquation = sf::BlendMode::Equation::Add;

	renderBuffer2.clear(sf::Color(0, 0, 0, 0));


	//Left
	if(dest_rect.x > 0) {
		tmpSprite.setTextureRect(sf::IntRect(0, 0, dest_rect.x, height));
		tmpSprite.setPosition(0, 0);
		renderBuffer2.draw(tmpSprite, blend);
	}

	//Right
	sf::Int32 rightEdgePos = dest_rect.x + dest_rect.width;
	if(width - rightEdgePos > 0) {
		tmpSprite.setTextureRect(sf::IntRect(static_cast<sf::Int32>(rightEdgePos), 0, static_cast<sf::Int32>(width - rightEdgePos), static_cast<sf::Int32>(height)));
		tmpSprite.setPosition(static_cast<float>(rightEdgePos), 0);
		renderBuffer2.draw(tmpSprite, blend);
	}

	//Top
	if(dest_rect.y > 0) {
		tmpSprite.setTextureRect(sf::IntRect(dest_rect.x, 0, dest_rect.width, dest_rect.y));
		tmpSprite.setPosition(static_cast<float>(dest_rect.x), 0.0f);
		renderBuffer2.draw(tmpSprite, blend);
	}

	//Bottom
	int bottomEdgePos = dest_rect.y + dest_rect.height;
	if(height - bottomEdgePos > 0) {
		tmpSprite.setTextureRect(sf::IntRect(dest_rect.x, bottomEdgePos, dest_rect.width, height - bottomEdgePos));
		tmpSprite.setPosition(static_cast<float>(dest_rect.x), static_cast<float>(bottomEdgePos));
		renderBuffer2.draw(tmpSprite, blend);
	}

	//Render changed section
	unsigned char* tmpBuffer = src_buffer + (src_row_span*src_rect.y) + (src_rect.x * 4);
	textureToUpdate.update(tmpBuffer,
						   static_cast<unsigned int>(src_row_span / 4),
						   static_cast<unsigned int>(dest_rect.height),
						   0,
						   0);
	tmpSprite.setTexture(textureToUpdate);

	tmpSprite.setPosition(static_cast<float>(dest_rect.x), static_cast<float>(dest_rect.y));
	tmpSprite.setTextureRect(sf::IntRect(0, 0, dest_rect.width, dest_rect.height));

    //Fix colors
	sf::RenderStates state;
	state.shader = &shader;
	state.blendMode = blend;

	renderBuffer2.draw(tmpSprite, state);
	renderBuffer2.display();

	//Copy to actual buffer
	renderBuffer.clear(sf::Color(0, 0, 0, 255));
	tmpSprite.setPosition(0, 0);
	tmpSprite.setTexture(renderBuffer2.getTexture());
	tmpSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	renderBuffer.draw(tmpSprite, blend);
	renderBuffer.display();
}

void CustomSurface::Scroll(int dx,
	int dy,
	const Awesomium::Rect& clip_rect)
{
	tmpSprite.setTexture(renderBuffer.getTexture());
	tmpSprite.setTextureRect(sf::IntRect(clip_rect.x, clip_rect.y, clip_rect.width - dx, clip_rect.height - dy));
	tmpSprite.setPosition(static_cast<float>(clip_rect.x + dx), static_cast<float>(clip_rect.y + dy));

	renderBuffer2.draw(tmpSprite);
	renderBuffer2.display();

	tmpSprite.setTexture(renderBuffer2.getTexture());
	tmpSprite.setTextureRect(sf::IntRect(0, 0, width, height));
	tmpSprite.setPosition(0, 0);
	renderBuffer.draw(tmpSprite);

	renderBuffer.display();
}