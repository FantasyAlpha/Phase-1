#include "TileMap.h"

void LoadLevel(TileMap *tileMap, char *levelPath)
{
	DataFile file;
	
	LoadFile(levelPath, &file);
	
	tileMap->Map = (unsigned char *)file.Data;
	tileMap->MapSize = file.Length;
	tileMap->Tiles = new Mesh[tileMap->MapSize];
	//UnloadFile(&file);
}

void ConstructMap(TileMap *tileMap, vec2 mapDimensions, vec2 tileSize)
{
	if (tileMap->MapSize != (int)(mapDimensions.x * mapDimensions.y))
	{
		std::cout << "Wrong tile map dimensions\n";
		system("PAUSE");
	}

	for (int i = 0; i < mapDimensions.y; i++)
	{
		for (int j = 0; j < mapDimensions.x; j++)
		{
			if (tileMap->Map[j + (i * (int)mapDimensions.x)] == '0')
			{		
				Color colors[4] = 
				{
					Color(1, 0, 0.5f, 1),
					Color(0, 0, 0.5f, 1),
					Color(1, 0, 0.5f, 1),
					Color(0, 0, 1, 1)
				};

				tileMap->Tiles[j + (i * (int)mapDimensions.x)] = CreateSprite(tileSize, vec3(j * tileSize.x, i * tileSize.y, 0), "resources\\textures\\tile1.png", colors);
			}

			if (tileMap->Map[j + (i * (int)mapDimensions.x)] == '1')
			{
				Color colors[4] =
				{
					Color(0, 0, 1, 1),
					Color(0, 1, 1, 1),
					Color(0, 0, 1, 1),
					Color(0, 1, 0, 1),
				};

				tileMap->Tiles[j + (i * (int)mapDimensions.x)] = CreateSprite(tileSize, vec3(j * tileSize.x, i * tileSize.y, 0), "resources\\textures\\tile3.png", colors);
			}			

		}
	}
}