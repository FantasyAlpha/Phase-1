#include "Game.h"

global_variable Mesh test;
global_variable Shader shader;

global_variable Transform transform;
global_variable Camera Cam;

float screenWidth = 1280;
float screenHeight = 720;

global_variable SpriteBatch Batch;

global_variable	Texture t;
//global_variable Mesh testMesh[10000];

//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	CalculateOrthoProjectionMatrix(&Cam, screenWidth, screenHeight, -0.1f, 500.0f);
	SetTexture(&test.MeshTexture, "resources\\textures\\tile2.png");
	//the mesh i will use
	//CreateSprite(&test, vec2(50, 50), vec3(0, 0, 0), &test.MeshTexture, &Color(1, 1, 1, 1), 1);
	//Use older shaders with old GLSL
#if GLSL_VERSION == ANCIENT_VERSION
	shader = Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	shader = Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");
#endif
	SetTexture(&t, "resources\\textures\\tile2.png");
	//CalculatePerspectiveProjection(&Cam, 80.0f, screenWidth, screenHeight, -0.1f, 500.0f);
	
	InitSpriteBatch(&Batch, 100000, MESH_TYPE::DYNAMIC_SPRITE_BATCH);
	BeginStoringInSpriteBatch(&Batch);
	for (int i = 0; i < 100000; i++)
	{
		//CreateSprite(&testMesh[i], vec2(50, 50), vec3(50 * i, 0, 0), &t, &Color(1, 1, 1, 1), 1);
		AddSpriteToBatch(&Batch, 1, &vec3((50 * i) , 0, 0), 1, &vec2(50, 50), 1, &t, 1, &Color(1, 1, 1, 1), 1);
	}
	EndStoringInSpriteBatch(&Batch);

//#define BATCH_SIZE 1000
//	vec3 pos[BATCH_SIZE];
//	vec2 siz[BATCH_SIZE];
//	for (int i = 0; i < BATCH_SIZE; i++)
//	{
//		pos[i] = vec3(0 + (i * 50), 0, 0);
//		siz[i] = vec2(50 , 50);
//	}
	//AddSpriteToBatch(&Batch, BATCH_SIZE, pos, BATCH_SIZE, siz, BATCH_SIZE, &t, BATCH_SIZE, &Color(1, 1, 1, 1), 1);
	//AddSpriteToBatch(&Batch, 10000, &vec3(0 , 0, 0), 1, &vec2(50, 50), 1, &t, 1, &Color(1, 1, 1, 1), 1);
	CalculateLookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	CalculateModelMatrix(&transform);

	//Activate the shader
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&transform, &Cam).GetElemets());
	//Draw the mesh
	/*for (int i = 0; i < 10000; i++)
	{
		DrawMesh(&testMesh[i]);
	}*/
	RenderSpriteBatch(&Batch);
}

Transform testTrans;


//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	int t[5] = {0, 0, 0, 0, 0};
	testTrans.Position.y = 0;
	if (input->UP.KeyDown)
	{
		t[0] = 0;
		t[1] = 2;
		t[2] = 4;
		t[3] = 6;
		t[4] = 8;
		testTrans.Position.y = 1;
		CalculateModelMatrix(&testTrans);
	}

	if (input->DOWN.KeyDown)
	{
		t[0] = 1;
		t[1] = 3;
		t[2] = 5;
		t[3] = 7;
		t[4] = 9;
		testTrans.Position.y = -1;
		CalculateModelMatrix(&testTrans);
	}

	if (input->RIGHT.KeyDown)
	{
		t[0] = 0;
		t[1] = 2;
		t[2] = 4;
		t[3] = 6;
		t[4] = 8;
		testTrans.Position.x = 1;
		CalculateModelMatrix(&testTrans);
	}

	if (input->LEFT.KeyDown)
	{
		t[0] = 1;
		t[1] = 3;
		t[2] = 5;
		t[3] = 7;
		t[4] = 9;
		testTrans.Position.x = -1;
		CalculateModelMatrix(&testTrans);
	}

	UpdateSpriteBatch(&Batch, &testTrans, 1, t, 5);
}
void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}