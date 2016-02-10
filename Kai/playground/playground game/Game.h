/*=======================================================================================================================
NOTE(kai): This file can:

1) Create a mesh
2) Create a shader
3) Draw the mesh

NOTE(kai): This is a test game
=======================================================================================================================*/

#pragma once

#ifdef GAME_DLL_EXPORTS
#define GAME_DLL extern "C" __declspec(dllexport) 
#else
#define GAME_DLL __declspec(dllimport) 
#endif

#include <Mesh.h>
#include <Shader.h>
#include <Transform.h>
#include "TileMap.h"
//
struct Game_Memory
{
	void *ScratchPad;
};

struct Game_Button
{
	int Button;

	bool KeyDown;
	bool KeyUp;
};

struct Game_Input
{
	Game_Button UP;
	Game_Button DOWN;
	Game_Button RIGHT;
	Game_Button LEFT;	
};

struct Game_Textures
{
	Texture Wall;
	Texture Floor;
	Texture Chess;

	Texture PlayerUp;
	Texture PlayerDown;
	Texture PlayerLeft;
	Texture PlayerRight;
};

Sprite Player;
TileMap map;

Transform transform;
Camera Cam;

Game_Resources Resources;
Game_Textures textures;

//Initialize the game
#define GAME_INIT(name) void name(void)
typedef GAME_INIT(game_init);

//Update the game
#define GAME_UPDATE(name) void name(Game_Input *input)
typedef GAME_UPDATE(game_update);

//Render the game
#define GAME_RENDER(name) void name(void)
typedef GAME_RENDER(game_render);

bool DetectCollision(TileMap *map, Sprite *player);
bool CheckCollisionAABB(Sprite sprite1, Sprite sprite2);

void CreateLevel1();
void CreateLevel2();
void InitializeResources();