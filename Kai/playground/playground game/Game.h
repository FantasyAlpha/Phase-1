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
#include<ctime>
#include <Mesh.h>
#include <Shader.h>
#include <Transform.h>
#include "TileMap.h"
#include <Entity.h>

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
	Game_Button _A;  // left
	Game_Button _D;   // right
	Game_Button _S;   //down
	Game_Button _W;   //up

	Game_Button space;
	Game_Button UP;
	Game_Button DOWN;
	Game_Button RIGHT;
	Game_Button LEFT;
};

struct Window_Dimensions
{
	float Width;
	float Height;
};

//Initialize the game
#define GAME_INIT(name) void name(Window_Dimensions dimensions)
typedef GAME_INIT(game_init);

//Update the game
#define GAME_UPDATE(name) void name(Game_Input *input,Window_Dimensions dimensions, float delta)
typedef GAME_UPDATE(game_update);

//Render the game
#define GAME_RENDER(name) void name(void)
typedef GAME_RENDER(game_render);

//compute time 
vec3 clamp(vec3 value, vec3 minimum, vec3 maximum);
double dotProduct(vec3 a, vec3 b);
void computetime(clock_t start, clock_t end);
void Move(Game_Input *input, Window_Dimensions dimensions, float delta);
void InitShaders();

void jumpHandle(Game_Input *input, float delta);
 
bool AABBvsAABB(Mesh *A , Mesh *B);
bool checkBoxCircleCollision(Mesh *circle, Mesh *box);

float Approach(float flGoal, float flCurrent, float delta);






//************************************************** prevuis trial *********************************************************

//enum Direction_1 {
//	UP,
//	RIGHT
//};
//enum Direction_2{
//	DOWN,
//	LEFT
//};
//
//enum Direction{
//	UP = 6,
//	RIGHT,
//	DOWN,
//	LEFT
//};

//Direction_1 VectorDirection_1(vec3 target);
//Direction_2 VectorDirection_2(vec3 target);
//Direction VectorDirection(vec3 target);
void DoBoxesCollision();
bool checkBoxescollision(Mesh *one, Mesh *two); // AABB - AABB collision
void DoBoxCircleCollision();
void DoAABB_AABB();

//bool AABBvsAABB(Mesh *one, Mesh *two);
//*******************************************************************************************************************************