#include "TileMap.h"

void LoadLevel(TileMap *tileMap, char *levelPath)
{
	DataFile file;
	
	LoadFile(levelPath, &file);
	
	unsigned char *temp = (unsigned char *)file.Data;
	
	int x = 0;

	for (int i = 0; i < file.Length; i++)
	{ 		
		if (temp[i] == '\n' || temp[i] == '\r')
		{
			++x;
		}
	}

	tileMap->Map = new unsigned char[file.Length - x];

	int y = 0;

	for (int i = 0; i < file.Length; i++)
	{
		if (temp[i] == '\n' || temp[i] == '\r')
		{
			continue;
		}
		tileMap->Map[y] = temp[i];
		y++;
		tileMap->MapSize++;
	}

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
		for (int j = 0; j < mapDimensions.x + 2; j++)
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