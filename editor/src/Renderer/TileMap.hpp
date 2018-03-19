#pragma once

//Dependencies
#include <SFML/Graphics.hpp>

// Handles the massive tile-map. Can be thought as the heart of this
// project.
class TileMap{
	//Tile count on one edge of the map
	sf::Uint16 tileCount;

	std::string textureFileName;

	//Max tiles to render
	static const sf::Uint16 maxTile;

	//Used to store atlas size in tiles
	sf::Vector2<sf::Uint8> _atlasDims;

	static const sf::Int16 tileSize ;
	static const sf::Vector2<sf::Int16> actualTileSize;
	static const sf::Vector2f tileCenterOffset;

	//Mini map edge size
	static const sf::Uint16 miniMapSize;

	sf::Uint8* tileData;
	sf::Uint8* tileAverageColors;

	sf::Texture _miniMapTexture;
	sf::Sprite* sprites;
	sf::Sprite miniMapSprite; //To be displayed as a low res version

	//TODO: move texture outside
	sf::Texture _atlas;

public:
	TileMap(sf::Uint16 size);

	void create(const sf::String atlasFile, bool isEmpty);

	void setTile(sf::Vector2f worldPosition, sf::Vector2<sf::Uint8> tile);
	void setTileBrush(sf::Vector2f worldPosition, sf::Vector2<sf::Uint8> tile);

	void draw(sf::RenderWindow* window, sf::Shader* shader = nullptr) const;

	void reset(bool isEmpty);
	void save(std::ofstream& file) const;
	void load(std::ifstream& file);

	void calculateMiniMap();
	void calculateMiniMap(const sf::Uint16 startX, const sf::Uint16 startY, const sf::Uint16 width, const sf::Uint16 height);

	//Constant references (read only)
	const sf::Texture &miniMapTexture = _miniMapTexture;
	const sf::Texture &atlas = _atlas;
	const sf::Vector2<sf::Uint8> &atlasDims = _atlasDims;

	~TileMap();

private:
	TileMap(const TileMap&);
	TileMap& operator=(const TileMap&);
};