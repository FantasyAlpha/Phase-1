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
//
struct Game_Memory
{
	void *ScratchPad;
};

//Mesh instance
Mesh mesh;

//Shader instance
Shader *shader;

Transform transform;
Camera Cam;

//Initialize the game
#define GAME_INIT(name) void name(void)
typedef GAME_INIT(game_init);

//Update the game
#define GAME_UPDATE(name) void name(void)
typedef GAME_UPDATE(game_update);

//Render the game
#define GAME_RENDER(name) void name(void)
typedef GAME_RENDER(game_render);