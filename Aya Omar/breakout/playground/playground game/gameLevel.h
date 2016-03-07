#pragma once 

//1 - load the level 
//2- configure and intilize the level (prepare render)
// 3- draw or render

#include <vector>

#include <GL/glew.h>
#include <Math\vec3.h>
#include <Math\vec2.h>
#include <Mesh.h>

#include "GameObject.h"



// level is game object type  which is collection of tiles (rows) of bricks which needed to be loaded from a file and processed 

/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.


struct Gamelevel {

	// Level state
	std::vector<GameObject> Bricks;
	Mesh * levelMesh;
	std::vector<Mesh> levelMeshes;

};




// we need loadlevel is public to intialize it from the game class 
// we load the file of the level and turn it to 2d matrix of gluint represent the tilecode (0,1,2,3,.......)
void loadLevel(Gamelevel *level, Mesh * levelmesh, const GLchar * fileName, float levelweidth, float levelHeight);

void createLevel(Gamelevel * level, Mesh * levelmesh, std::vector<std::vector<GLuint>>tileData, float levelwidth, float levelHeight);




// Check if the level is completed (all non-solid tiles are destroyed)
GLboolean IsCompleted(Gamelevel * level);

