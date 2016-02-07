#include "Game.h"

//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	//Square vertices (cordinates are from -1 to 1 in both x and y and the origin is at the center)
	Vertex vertices[] = 
	{ 
		Vertex(Position(0.5f, 0.5f, 0), Color(0, 0, 1, 1), TexCoords(1, 1)),		//TOP RIGHT
		Vertex(Position(0.5f, -0.5f, 0), Color(0, 1, 0, 1), TexCoords(1, 0)),		//BOTTOM RIGHT
		Vertex(Position(-0.5f, -0.5f, 0), Color(0, 0, 1, 1), TexCoords(0, 0)),		//BOTTOM LEFT
		Vertex(Position(-0.5f, 0.5f, 0), Color(1, 0, 1, 1), TexCoords(0, 1)),		//TOP LEFT
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
	shader = new Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	shader = new Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");
#endif

	//the mesh i will use
	LoadMesh(&mesh, vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
	SetTexture(&mesh, "resources\\textures\\test.png");

	transform.Position = vec3(0, 0, 0);
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	//Activate the shader
	shader->Activate();
	CalculateModelMatrix(&transform);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, transform.ModelMatrix.GetElemets());

	//Draw the mesh
	DrawMesh(&mesh);
}

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{

}