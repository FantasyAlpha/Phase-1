#pragma once

#include <Mesh.h>
#include <Utilities.h>

struct TileMap
{
	unsigned char *Map;
	int MapSize;

	Mesh *Tiles;
};

void LoadLevel(TileMap *tileMap, char *levelPath);
void ConstructMap(TileMap *tileMap, vec2 mapDimensions, vec2 tileSize);