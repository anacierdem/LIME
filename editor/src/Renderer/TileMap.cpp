#include <fstream>
#include <math.h>
#include "TileMap.hpp"
#include "../Misc/Util.hpp"
#include "../globals.hpp"
#include "../Misc/SaveMethods.hpp"

//TODO: need a better constructor & copy constructor
TileMap::TileMap(const sf::Uint16 size) {
	//Set parameters
	tileCount = size;
}

void TileMap::create(const sf::String atlasFile, bool isEmpty) {

    textureFileName = atlasFile;

	sf::Image loadedImage;

	if (!loadedImage.loadFromFile(atlasFile))	{
		//error
	}
	else {
		sf::Vector2u atlasSize = loadedImage.getSize();

		_atlasDims.x = static_cast<sf::Uint8>(floor(atlasSize.x / static_cast<float>(actualTileSize.x)));
		_atlasDims.y = static_cast<sf::Uint8>(floor(atlasSize.y / static_cast<float>(actualTileSize.y)));

		tileAverageColors = new sf::Uint8[atlasDims.x * atlasDims.y * 4];
		_miniMapTexture.create(miniMapSize, miniMapSize);

		const sf::Uint8* pixels = loadedImage.getPixelsPtr();

		sf::Uint32 red = 0;
		sf::Uint32 green = 0;
		sf::Uint32 blue = 0;
		sf::Uint32 alpha = 0;

		sf::Uint32 pixelCount = 0;

		//For each tile in atlas
		for (sf::Uint32 atlasX = 0; atlasX < atlasDims.x; atlasX++){
			for (sf::Uint32 altasY = 0; altasY < atlasDims.y; altasY++){

				//For each pixel in that tile
				for (sf::Uint32 x = atlasX*actualTileSize.x; x < atlasX*actualTileSize.x + actualTileSize.x; x++){
					for (sf::Uint32 y = altasY*actualTileSize.y; y < altasY*actualTileSize.y + actualTileSize.y; y++) {
						sf::Uint32 index = (atlasSize.x * y + x) * 4;

						//If not transparent
						if (pixels[index + 3] == 255) {
							red += pixels[index];
							green += pixels[index + 1];
							blue += pixels[index + 2];
							alpha += pixels[index + 3];
							pixelCount++;
						}
					}
				}

				//Calculate average color for this tile
				sf::Uint32 averageIndex = (atlasDims.x * altasY + atlasX) * 4;
				tileAverageColors[averageIndex] = static_cast<sf::Uint8>(round(red / static_cast<float>(pixelCount)));
				tileAverageColors[averageIndex + 1] = static_cast<sf::Uint8>(round(green / static_cast<float>(pixelCount)));
				tileAverageColors[averageIndex + 2] = static_cast<sf::Uint8>(round(blue / static_cast<float>(pixelCount)));
				tileAverageColors[averageIndex + 3] = static_cast<sf::Uint8>(round(alpha / static_cast<float>(pixelCount)));

				//Reset counters
				red = 0;
				green = 0;
				blue = 0;
				alpha = 0;
				pixelCount = 0;
			}
		}

		//Load into texture
		_atlas.loadFromImage(loadedImage);

		tileData = new sf::Uint8[tileCount*tileCount * 2];
		//TODO: performance improvement: change with diamond shaped mesh - reduce fill rate
		sprites = new sf::Sprite[maxTile];

		//Set up low res version
		miniMapSprite.setTexture(miniMapTexture);
		miniMapSprite.setPosition(0.0f, 0.0f);
		float scale = static_cast<float>(tileCount*tileSize / sqrt(2.0f)) / miniMapSize;
		miniMapSprite.setScale(scale, scale);

		//init sprites
		for (int i = 0; i<maxTile; i++){
			sprites[i].setTexture(atlas);
			sprites[i].setOrigin(tileCenterOffset);
		}

		//Reset map data
		reset(isEmpty);
	}
}

void TileMap::reset(bool isEmpty) {
	sf::Uint8 initialTileX = 0;
	sf::Uint8 initialTileY = 0;

	for (int i = 0; i<tileCount*tileCount; i++) {
		tileData[i * 2] = initialTileX;
		tileData[i * 2 + 1] = initialTileY;

		sf::Uint8 minTile = 7, maxTile = 7;
		if (!isEmpty) {
			minTile = 2;
			maxTile = 6;
		}

		initialTileX = static_cast<sf::Uint8>(minTile + (rand() % (maxTile - minTile + 1)));
		initialTileY = 0;
	}

	//Calculate whole minimap
	calculateMiniMap();
}

void TileMap::save(std::ofstream& file) const {

	Serialization::save(file, &textureFileName);

    Serialization::save(file, &tileCount);
    Serialization::save(file, tileData, tileCount*tileCount * 2);
    Serialization::save(file, &_atlasDims);
	Serialization::save(file, tileAverageColors, atlasDims.x * atlasDims.y * 4);
}

void TileMap::load(std::ifstream& file) {

    delete[] tileAverageColors;
    delete[] tileData;
    delete[] sprites;

	Serialization::load(file, &textureFileName);

    create(textureFileName, true);

    Serialization::load(file, &tileCount);
    Serialization::load(file, tileData, tileCount*tileCount * 2);
    Serialization::load(file, &_atlasDims);
    Serialization::load(file, tileAverageColors, atlasDims.x * atlasDims.y * 4);

	//Re-calculate minimap
	calculateMiniMap();
}

void TileMap::setTile(sf::Vector2f worldPosition, sf::Vector2<sf::Uint8> tile) {

	//Transform to real coordinates
	worldPosition = Util::isoToTwoD(worldPosition);

	sf::Uint16 xPos = static_cast<sf::Uint16>(floor(worldPosition.x / static_cast<float>(tileSize)));
	sf::Uint16 yPos = static_cast<sf::Uint16>(floor(worldPosition.y / static_cast<float>(tileSize)));

	//Update if inside map area
	if (xPos >= 0 && yPos >= 0 && xPos < tileCount && yPos < tileCount) {
		sf::Uint64 index = tileCount * yPos + xPos;

		tileData[index * 2] = tile.x;
		tileData[index * 2 + 1] = tile.y;

		//Update only the affected region
		calculateMiniMap(xPos, yPos, 1, 1);
	}
}

void TileMap::setTileBrush(sf::Vector2f worldPosition, sf::Vector2<sf::Uint8> tile) {

	const sf::Uint16 brushSize = 9;

	//Transform coordinates
	worldPosition = Util::isoToTwoD(worldPosition);

	sf::Int32 centerXPos = static_cast<sf::Int32>(floor(worldPosition.x / static_cast<float>(tileSize)));
	sf::Int32 centerYPos = static_cast<sf::Int32>(floor(worldPosition.y / static_cast<float>(tileSize)));

	//Update all values if inside map area
	for (sf::Int32 xPos = static_cast<sf::Int32>(centerXPos-floor(brushSize/2.0f)); xPos <= centerXPos+floor(brushSize/2.0f); xPos++){
		for (sf::Int32 yPos = static_cast<sf::Int32>(centerYPos-floor(brushSize/2.0f)); yPos <= centerYPos+floor(brushSize/2.0f); yPos++) {

			if (xPos >= 0 && yPos >= 0 && xPos < tileCount && yPos < tileCount) {
				sf::Uint64 index = static_cast<sf::Uint64>(tileCount * yPos + xPos);

				tileData[index * 2] = tile.x;
				tileData[index * 2 + 1] = tile.y;
			}
		}
	}

	//Update only the affected region
	calculateMiniMap( static_cast<sf::Uint16>(centerXPos-floor(brushSize/2.0f)), sf::Uint16(centerYPos-floor(brushSize/2.0f)), brushSize, brushSize);
}

void TileMap::calculateMiniMap(sf::Uint16 startX, sf::Uint16 startY, sf::Uint16 width, sf::Uint16 height) {

	//Update only the affected region
	float xRatio = startX / (tileCount-1.0f);
	float yRatio = startY / (tileCount-1.0f);

	float wRatio = width / static_cast<float>(tileCount);
	float hRatio = height / static_cast<float>(tileCount);

	sf::Uint16 textureX = static_cast<sf::Uint16>(floor(xRatio * (miniMapSize - 1)));
	sf::Uint16 textureY = static_cast<sf::Uint16>(floor(yRatio * (miniMapSize - 1)));

	sf::Uint16 textureWidth = static_cast<sf::Uint16>(ceil(wRatio * miniMapSize));
	sf::Uint16 textureHeight = static_cast<sf::Uint16>(ceil(hRatio * miniMapSize));

	//Boundary checks
	textureX = textureX >= miniMapSize ? miniMapSize - static_cast<sf::Uint16>(1) : textureX;
	textureY = textureY >= miniMapSize ? miniMapSize - static_cast<sf::Uint16>(1) : textureY;

	textureX = textureX >= 0 ? textureX : static_cast<sf::Uint16>(0);
	textureY = textureY >= 0 ? textureY : static_cast<sf::Uint16>(0);

	if (textureX + textureWidth > miniMapSize) {
		textureWidth = miniMapSize - textureX;
	}

	if (textureY + textureHeight > miniMapSize) {
		textureHeight = miniMapSize - textureY;
	}

	sf::Uint16 tilesPerMiniMapPixel = static_cast<sf::Uint16>(tileCount / miniMapSize);

	sf::Uint8* miniMapColors = new sf::Uint8[textureWidth*textureHeight * 4];

	//Loop in minimap area
	for (sf::Uint16 miniMapX = textureX; miniMapX < textureWidth + textureX; miniMapX++) {
		for (sf::Uint16 miniMapY = textureY; miniMapY < textureHeight + textureY; miniMapY++) {

			sf::Uint32 red = 0;
			sf::Uint32 green = 0;
			sf::Uint32 blue = 0;
			sf::Uint32 alpha = 0;

			//Loop tiles in this area
			for (sf::Uint32 x = miniMapX * tilesPerMiniMapPixel; x < static_cast<sf::Uint32>(miniMapX * tilesPerMiniMapPixel + tilesPerMiniMapPixel) && x < tileCount; x++) {
				for (sf::Uint32 y = miniMapY * tilesPerMiniMapPixel; y < static_cast<sf::Uint32>(miniMapY * tilesPerMiniMapPixel + tilesPerMiniMapPixel) && y < tileCount; y++) {
					sf::Uint32 index = (tileCount * y + x);

					sf::Uint32 colorIndex = static_cast<sf::Uint32>(atlasDims.x * tileData[index * 2 + 1] + tileData[index * 2]) * 4;

					red += tileAverageColors[colorIndex];
					green += tileAverageColors[colorIndex + 1];
					blue += tileAverageColors[colorIndex + 2];
					alpha += tileAverageColors[colorIndex + 3];
				}
			}

			//Calculate and set average tile color
			sf::Uint32 count = tilesPerMiniMapPixel*tilesPerMiniMapPixel;
			sf::Uint32 miniMapIndex = static_cast<sf::Uint32>(textureWidth * (miniMapY - textureY) + (miniMapX - textureX)) * 4;
			miniMapColors[miniMapIndex] = static_cast<sf::Uint8>(round(red / static_cast<float>(count)));
			miniMapColors[miniMapIndex + 1] = static_cast<sf::Uint8>(round(green / static_cast<float>(count)));
			miniMapColors[miniMapIndex + 2] = static_cast<sf::Uint8>(round(blue / static_cast<float>(count)));
			miniMapColors[miniMapIndex + 3] = static_cast<sf::Uint8>(round(alpha / static_cast<float>(count)));
		}
	}

	//update texture
	_miniMapTexture.update(miniMapColors, textureWidth, textureHeight, textureX, textureY);
	delete [] miniMapColors;
}

void TileMap::calculateMiniMap() {
	//Calculate over entire map
	calculateMiniMap(0, 0, tileCount, tileCount);
}

void TileMap::draw(sf::RenderWindow* window, sf::Shader* shader) const {

	sf::Vector2i winSize(window->getSize());

	//Find positions coinciding to screen corners
	sf::Vector2f topLeft = Util::isoToTwoD(window->mapPixelToCoords(sf::Vector2i(0, 0)));
	sf::Vector2f bottomRight = Util::isoToTwoD(window->mapPixelToCoords(winSize));
	sf::Vector2f topRight = Util::isoToTwoD(window->mapPixelToCoords(sf::Vector2i(winSize.x, 0)));

	//Set shader
	sf::Shader::bind(shader);

	//Draw low res view
	sf::Transform t;
	t.scale(1.0f, 0.5f);
	t.rotate(45);

	sf::RenderStates states;
	states.transform = t;
	window->draw(miniMapSprite, states);

	sf::Uint32 i = 0;

	//TODO allow drawing on edge cases
	topLeft.x = topLeft.x >= 0 ? topLeft.x : 0;
	topLeft.y = topLeft.y >= 0 ? topLeft.y : 0;

	sf::Int32 startIndex = static_cast<sf::Int32>((floor(topLeft.y / tileSize) /*- 1*/) * tileCount + floor(topLeft.x / tileSize) /*- 2*/);
	sf::Int32 currentIndex = startIndex;

	sf::Uint32 colCount = static_cast<sf::Uint32>(ceil((bottomRight.x - topLeft.x) / tileSize));
	sf::Uint32 rowCount = static_cast<sf::Uint32>((ceil((bottomRight.y - topRight.y) / tileSize) /*+ 2*/) * 2) /*+ 1*/;

	sf::Uint32 currentCol = 0;
	sf::Uint32 currentRow = 0;

	while(i < maxTile) {

		//If inside tile map
		if (currentIndex >= 0 && currentIndex < tileCount*tileCount) {
			int textureCol = tileData[currentIndex*2];
			int textureRow = tileData[currentIndex*2 + 1];

			//Set tile texture and render
			sprites[i].setTextureRect(sf::IntRect(textureCol * actualTileSize.x, textureRow * actualTileSize.y, actualTileSize.x, actualTileSize.y));

			sf::Uint16 col = static_cast<sf::Uint16>((currentIndex % tileCount));
			sf::Uint16 row = static_cast<sf::Uint16>(floor(currentIndex / tileCount));

			sf::Vector2f pos = Util::twoDToIso(sf::Vector2f(static_cast<float>(col * tileSize), static_cast<float>(row * tileSize)));
			sprites[i].setPosition(pos);

			//TODO: batch these - Possible performance gain
			window->draw(sprites[i]);
		}

		//TODO: change memory scheme + lower cahce misses  - Possible performance gain
		currentIndex -= (tileCount-1);
		currentCol++;

		if(currentCol >= colCount){
			currentCol = 0;
			currentRow++;
			if(currentRow % 2 == 0) {
				currentIndex = startIndex + (currentRow/2) * (tileCount + 1);
			}else {
				currentIndex = startIndex + (currentRow/2) * (tileCount + 1) + 1;
			}
		}

		if(currentRow >= rowCount){
			break;
		}

		i++;
	}

    sf::Shader::bind(nullptr);
}

TileMap::~TileMap() {
	delete[] tileAverageColors;
	delete[] tileData;
	delete[] sprites;
}

const sf::Int16 TileMap::tileSize = 128;
const sf::Vector2<sf::Int16> TileMap::actualTileSize = sf::Vector2<sf::Int16>(128, 128);
const sf::Vector2f TileMap::tileCenterOffset = sf::Vector2f(64.0f, 64.0f);
const sf::Uint16 TileMap::miniMapSize = 1024;

const sf::Uint16 TileMap::maxTile = MAX_TILE_COUNT;