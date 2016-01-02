#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
	if (m_Mesh)
	{
		delete m_Mesh;
	}

	if (m_Shader)
	{
		delete m_Shader;
	}
}

//Initialize the game
void Game::Init()
{
	//Square vertices (cordinates are from -1 to 1 in both x and y and the origin is at the center)
	Vertex vertices[] = 
	{ 
		Vertex(Position(0.5f, 0.5f, 0), Color(0, 0, 1, 1)),		//TOP RIGHT
		Vertex(Position(0.5f, -0.5f, 0), Color(0, 1, 0, 1)),	//BOTTOM RIGHT
		Vertex(Position(-0.5f, -0.5f, 0), Color(0, 0, 1, 1)),	//BOTTOM LEFT
		Vertex(Position(-0.5f, 0.5f, 0), Color(1, 0, 1, 1)),	//TOP LEFT
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
	m_Shader = new Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	m_Shader = new Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");
#endif

	//the mesh i will use
	m_Mesh = new Mesh(vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
	
}

//Render the game
void Game::Render()
{
	//Activate the shader
	m_Shader->Activate();
	//Draw the mesh
	m_Mesh->Draw();
}

//Update the game
void Game::Update()
{

}