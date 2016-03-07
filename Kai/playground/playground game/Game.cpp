#include "Game.h"

global_variable Mesh Player;
global_variable Mesh Ground;
global_variable Mesh Wall;
global_variable Shader shader;

global_variable Camera Cam;


//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	InitShaders();

	CalculateOrthoProjectionMatrix(&Cam, dimensions.Width, dimensions.Height, -0.1f, 500.0f);
	//CalculatePerspectiveProjection(&Cam, 80.0f, dimensions.Width, dimensions.Height, -0.1f, 500.0f);
	CalculateLookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));

	Player = CreateSprite(vec2(50, 50), vec3(0, 0, 0), &SetTexture("resources\\textures\\empty.png"), &Color(0, 0.5f, 1, 1), 1);
	Ground = CreateSprite(vec2(dimensions.Width, 100), vec3(-dimensions.Width / 2.0f, -dimensions.Height / 2.0f, 0), &SetTexture("resources\\textures\\empty.png"), &Color(0, 1, 1, 1), 1);
	Wall = CreateSprite(vec2(dimensions.Width, dimensions.Height), vec3(-dimensions.Width / 2.0f, -dimensions.Height / 2.0f, 0), &SetTexture("resources\\textures\\empty.png"), &Color(0.2f, 0.2f, 0.2f, 1), 1);
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	CalculateModelMatrix(&Player.GlobalTransform);

	//Activate the shader
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&Wall.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&Wall);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&Ground.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&Ground);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&Player.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&Player);
}

Transform testTrans;


//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	
}
void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

void InitShaders()
{
#if GLSL_VERSION == ANCIENT_VERSION
	shader = Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	shader = Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");
#endif
}