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

void ConstructMap(TileMap *tileMap, Game_Resources *resources, TileCase *cases, int caseCount, vec3 position, vec2 mapDimensions, vec2 tileSize)
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

			tileMap->Tiles[index].TileSprite.GlobalTransform.Position = vec3(position.x + (j * tileSize.x)
																			, position.y + (i * tileSize.y)
																			, position.z);

			for (int k = 0; k < caseCount; k++)
			{
				if (tileMap->Map[index] == cases[k].Case)
				{
					CreateSprite(&tileMap->Tiles[index].TileSprite, vec2(tileSize.x, tileSize.y), tileMap->Tiles[index].TileSprite.GlobalTransform.Position, &cases[k].CaseTexture, cases[k].Colors, 4);

					tileMap->Tiles[index].Type = k;
				}
			}
		}
	}
}