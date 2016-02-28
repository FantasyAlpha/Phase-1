/*=======================================================================================================================
NOTE(kai): This file can:

1) Create a mesh
2) Create a shader
3) Draw the mesh

NOTE(kai): This is a test game
=======================================================================================================================*/

#pragma once

#include "Mesh.h"
#include "Shader.h"

class Game
{
public:
	Game();
	~Game();

public:
	//Initialize the game
	void Init();

	//Render the game
	void Render();

	//Update the game
	void Update();

private:
	//Mesh instance
	Mesh *m_Mesh;

	//Shader instance
	Shader *m_Shader;
};

