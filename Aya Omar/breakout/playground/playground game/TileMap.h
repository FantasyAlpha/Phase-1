#pragma once

#include <Mesh.h>
#include <Utilities.h>
#include "ResourceManager.h"

struct Tile
{
	Mesh TileSprite;
	int Type;
};

struct TileMap
{
	unsigned char *Map;
	int MapSize;

	Tile *Tiles;
};

struct TileCase
{
	Color Colors[4];
	Texture CaseTexture;
	unsigned char Case;
};

void ConstructMap(TileMap *tileMap, Game_Resources *resources, TileCase *cases, int caseCount, vec3 position, vec2 mapDimensions, vec2 tileSize);
void LoadLevel(TileMap *tileMap, char *levelPath);