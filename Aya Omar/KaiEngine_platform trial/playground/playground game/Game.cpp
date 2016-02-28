#include "Game.h"

global_variable Mesh test;
global_variable Shader shader;

global_variable Transform transform;
global_variable Camera Cam;

float screenWidth = 1280;
float screenHeight = 720;

//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	GLenum error = glGetError();
	SetTexture(&test.MeshTexture, "resources\\textures\\tile2.png");
	error = glGetError();
	//the mesh i will use
	CreateSprite(&test, vec2(50, 50), vec3(0, 0, 0), &test.MeshTexture, &Color(1, 1, 1, 1), 1);
	error = glGetError();
	//Use older shaders with old GLSL
#if GLSL_VERSION == ANCIENT_VERSION
	shader = Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
	error = glGetError();
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	shader = Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");
#endif

	//CalculatePerspectiveProjection(&Cam, 80.0f, screenWidth, screenHeight, -0.1f, 500.0f);
	CalculateOrthoProjectionMatrix(&Cam, screenWidth, screenHeight, -0.1f, 500.0f);
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	clock_t startRender = clock();
	CalculateLookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));
	CalculateModelMatrix(&transform);
	
	//Activate the shader
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&transform, &Cam).GetElemets());
	GLenum error = glGetError();
	//Draw the mesh
	DrawMesh(&test);
	error = glGetError();
	clock_t endReender = clock();
	computetime(startRender, endReender);
}

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{

}
void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

	std::cout << "game render time is : "<<elapsedtime << "ms" << std::endl;

}