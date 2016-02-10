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

	tileMap->Tiles = new Tile[tileMap->MapSize];
	//UnloadFile(&file);
}

void ConstructMap(TileMap *tileMap, Game_Resources *resources, vec3 position, vec2 mapDimensions, vec2 tileSize)
{
	if (tileMap->MapSize != (int)(mapDimensions.x * mapDimensions.y))
	{
		std::cout << "Wrong tile map dimensions\n";
		system("PAUSE");
	}

	for (float i = 0; i < mapDimensions.y; i++)
	{
		for (float j = 0; j < mapDimensions.x; j++)
		{
			int index = (int)(j + (i * (int)mapDimensions.x));

			tileMap->Tiles[index].TileSprite.Position = vec3(position.x + (j * tileSize.x)
															, position.y + (i * tileSize.y)
															, position.z);
			if (tileMap->Map[index] == '0')
			{		
				Color colors[4] = 
				{
					Color(1, 0, 0.5f, 1),
					Color(0, 0, 0.5f, 1),
					Color(1, 0, 0.5f, 1),
					Color(0, 0, 1, 1)
				};

				Tile t;
				CreateSprite(&t.TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Wall"), colors, 4);
				t.Type = 0;
				tileMap->tiles.push_back(t);
				CreateSprite(&tileMap->Tiles[index].TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Wall"), colors, 4);
				
				tileMap->Tiles[index].Type = 0;
			}

			if (tileMap->Map[index] == '1')
			{
				Color colors[4] =
				{
					Color(0, 0, 1, 1),
					Color(0, 1, 1, 1),
					Color(0, 0, 1, 1),
					Color(0, 1, 0, 1),
				};				
				
				Tile t;
				CreateSprite(&t.TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Floor"), colors, 4);
				t.Type = 1;
				tileMap->tiles.push_back(t);
				CreateSprite(&tileMap->Tiles[index].TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Floor"), colors, 4);

				tileMap->Tiles[index].Type = 1;
			}		

			if (tileMap->Map[index] == '2')
			{
				Color colors[4] =
				{
					Color(0, 0, 1, 1),
					Color(0.0f, 0.2f, 0.8f, 1),
					Color(1, 0, 1, 1),
					Color(0, 0, 1, 1),
				};

				Tile t;
				CreateSprite(&t.TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Floor"), colors, 4);
				t.Type = 2;
				tileMap->tiles.push_back(t);
				CreateSprite(&tileMap->Tiles[index].TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Floor"), colors, 4);

				tileMap->Tiles[index].Type = 2;
			}

			if (tileMap->Map[index] == '3')
			{
				Color colors[4] =
				{
					Color(1, 0, 0, 1),
					Color(1, 0, 0, 1),
					Color(1, 0, 1, 1),
					Color(0, 0, 1, 1),
				};

				Tile t;
				CreateSprite(&t.TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Wall"), colors, 4);
				t.Type = 3;
				tileMap->tiles.push_back(t);
				CreateSprite(&tileMap->Tiles[index].TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.Position, GetTexture(resources, "Wall"), colors, 4);

				tileMap->Tiles[index].Type = 3;
			}

		}
	}
}