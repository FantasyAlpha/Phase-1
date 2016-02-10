#pragma once

#include <Mesh.h>
#include <Utilities.h>
#include "ResourceManager.h"

struct Tile
{
	Sprite TileSprite;
	int Type;
};

struct TileMap
{
	unsigned char *Map;
	int MapSize;

	std::vector<Tile> tiles;
	Tile *Tiles;
};

void ConstructMap(TileMap *tileMap, Game_Resources *resources, vec3 position, vec2 mapDimensions, vec2 tileSize);
void LoadLevel(TileMap *tileMap, char *levelPath);