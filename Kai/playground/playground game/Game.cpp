#include "Game.h"

//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	//Square vertices (cordinates are from -1 to 1 in both x and y and the origin is at the center)
	Vertex vertices[] = 
	{ 
		Vertex(Position(50.0f, 50.0f, 0), Color(0, 0, 1, 1), TexCoords(1, 1)),		//TOP RIGHT
		Vertex(Position(50.0f, -50.0f, 0), Color(0, 1, 0, 1), TexCoords(1, 0)),		//BOTTOM RIGHT
		Vertex(Position(-50.0f, -50.0f, 0), Color(0, 0, 1, 1), TexCoords(0, 0)),		//BOTTOM LEFT
		Vertex(Position(-50.0f, 50.0f, 0), Color(1, 0, 1, 1), TexCoords(0, 1)),		//TOP LEFT
	};

	//Order of vertices that will be drawn
	unsigned int indices[] = 
	{
		0,
		1, 
		3,
		1,
		2, 
		3,
	};

	//Use older shaders with old GLSL
#if GLSL_VERSION == ANCIENT_VERSION
	AddShader(&Resources, new Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag"), "Basic Shader");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	AddShader(&Resources, new Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag"), "Basic Shader");
#endif
	textures = {};

	SetTexture(&textures.Wall, "resources\\textures\\tile1.png");
	AddTexture(&Resources, &textures.Wall, "Wall");
	
	SetTexture(&textures.Chess, "resources\\textures\\tile2.png");
	AddTexture(&Resources, &textures.Chess, "Chess");
	
	SetTexture(&textures.Floor, "resources\\textures\\tile3.png");
	AddTexture(&Resources, &textures.Floor, "Floor");
	
	SetTexture(&textures.PlayerDown, "resources\\textures\\player.png");
	AddTexture(&Resources, &textures.PlayerDown, "Player Down");
	
	SetTexture(&textures.PlayerLeft, "resources\\textures\\player1.png");
	AddTexture(&Resources, &textures.PlayerLeft, "Player Left");
	
	SetTexture(&textures.PlayerRight, "resources\\textures\\player2.png");
	AddTexture(&Resources, &textures.PlayerRight, "Player Right");
	
	SetTexture(&textures.PlayerUp, "resources\\textures\\player3.png");
	AddTexture(&Resources, &textures.PlayerUp, "Player Up");

	float screenWidth = 1280.f;
	float screenHeight = 720.0f;
	float tileSize = 70.0f;
	float tileCountX = 16;
	float tileCountY = 9;

	//the mesh i will use
	//LoadMesh(&mesh, vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
	//SetTexture(&mesh, "resources\\textures\\tile2.png");

	Cam = {};

	CalculatePerspectiveProjection(&Cam, 80.0f, screenWidth / 2.0f, screenHeight / 2.0f, 0.1f, 500.0f);
	CalculateOrthoProjectionMatrix(&Cam, screenWidth / 2.0f, screenHeight / 2.0f, 0.1f, 500.0f);

	Color colors[4] =
	{
		Color(1, 1, 0, 1),
		Color(1, 0, 0, 1),
		Color(1, 1, 0, 1),
		Color(0, 1, 1, 1),
	};

	vec3 ORIGIN = vec3(-(tileSize * tileCountX) / 2.0f, -(tileSize * tileCountY) / 2.0f, -50);

	Player.LocalTransform.Position = vec3(0, 0, -50);
	vec2 playerSize = vec2(tileSize - (tileSize / 8.0f), tileSize - (tileSize / 8.0f));
	CreateSprite(&Player, playerSize, vec3(140, 150, 0), GetTexture(&Resources, "Player Down"), colors, 4);

	//transform.Position = vec3(-(tileSize * tileCountX) / 2.15f, -(tileSize * tileCountY) / 2.22f, 50);
	//transform.Scale = vec3(1, 1, 1);

	map = {};
	LoadLevel(&map, "resources\\tilemaps\\map1.txt");	

	ConstructMap(&map, &Resources, ORIGIN, vec2(tileCountX, tileCountY), vec2(tileSize, tileSize));
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	//Activate the shader
	GetShader(&Resources, "Basic Shader")->Activate();
	CalculateLookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));
	
	CalculateModelMatrix(&transform);
	glUniformMatrix4fv(glGetUniformLocation(GetShader(&Resources, "Basic Shader")->GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&transform, &Cam).GetElemets());

	//Draw the mesh
	for (int i = 0; i < map.MapSize; i++)
	{
		DrawSprite(&map.tiles[i].TileSprite);
	}

	CalculateModelMatrix(&Player.LocalTransform);
	glUniformMatrix4fv(glGetUniformLocation(GetShader(&Resources, "Basic Shader")->GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&Player.LocalTransform, &Cam).GetElemets());
	DrawSprite(&Player);
}

vec3 PlayerPos = Player.Position;

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	float speed = 1.0f;
	float moveX = 0.0f;
	float moveY = 0.0f;
	

	if (input->RIGHT.KeyDown)
	{
		Player.SpriteTexture = *GetTexture(&Resources, "Player Left");
		
		moveX = Player.LocalTransform.Position.x;
		Player.LocalTransform.Position.x = Player.LocalTransform.Position.x + speed;
		moveX = Player.LocalTransform.Position.x - moveX;
		Player.Position.x += moveX;
	}

	if (input->LEFT.KeyDown)
	{
		Player.SpriteTexture = *GetTexture(&Resources, "Player Right");
		
		moveX = Player.LocalTransform.Position.x;
		Player.LocalTransform.Position.x = Player.LocalTransform.Position.x - speed;
		moveX = Player.LocalTransform.Position.x - moveX;
		Player.Position.x += moveX;
	}

	if (input->UP.KeyDown)
	{
		Player.SpriteTexture = *GetTexture(&Resources, "Player Up");
		
		moveY = Player.LocalTransform.Position.y;
		Player.LocalTransform.Position.y = Player.LocalTransform.Position.y + speed;
		moveY = Player.LocalTransform.Position.y - moveY;
		Player.Position.y += moveY;
	}

	if (input->DOWN.KeyDown)
	{
		Player.SpriteTexture = *GetTexture(&Resources, "Player Down");
		
		moveY = Player.LocalTransform.Position.y;
		Player.LocalTransform.Position.y = Player.LocalTransform.Position.y - speed;
		moveY = Player.LocalTransform.Position.y - moveY;
		Player.Position.y += moveY;
	}

	if (DetectCollision(&map, &Player))
	{
		moveX *= -1.1f;
		moveY *= -1.1f;
		Player.LocalTransform.Position.x += moveX; 
		Player.LocalTransform.Position.y += moveY;
		Player.Position.x += moveX;
		Player.Position.y += moveY;
	}
}

bool CheckCollisionAABB(Sprite *sprite1, Sprite *sprite2)
{
	bool collisionOnX = ((sprite1->Position.x + sprite1->Size.x) > sprite2->Position.x)
		&& ((sprite2->Position.x + sprite2->Size.x) >= sprite1->Position.x);

	bool collisionOnY = ((sprite1->Position.y + sprite1->Size.y) > sprite2->Position.y)
		&& ((sprite2->Position.y + sprite2->Size.y) >= sprite1->Position.y);

	return collisionOnX && collisionOnY;
}

bool DetectCollision(TileMap *map, Sprite *player)
{
	for (int i = 0; i < map->MapSize; i++)
	{
		if (map->tiles[i].Type == 1)
		{
			continue;
		}

		if (CheckCollisionAABB(player, &map->tiles[i].TileSprite))
		{
			map->tiles[i].TileSprite.SpriteTexture = *GetTexture(&Resources, "Chess");
			return true;
		}
	}

	return false;
}