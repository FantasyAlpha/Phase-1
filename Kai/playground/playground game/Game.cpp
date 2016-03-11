#include "Game.h"

global_variable Mesh TestMesh;
global_variable Material TestMaterial;
global_variable Shader shader;

global_variable Transform transform;
global_variable Camera Cam;

float screenWidth;
float screenHeight;

global_variable World TestWorld;
global_variable Game_Memory MainMemory;
global_variable MainAllocator MainAllocatorSystem;

//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	{
		screenWidth = dimensions.Width;
		screenHeight = dimensions.Height;
	}

	{
		InitShaders();
		InitCamera();
	}

	{
		StartMemorySystem(&MainAllocatorSystem, AllocatorTypes::STACK_ALLOCATOR, Megabytes(500), 4);
	}

	{
		InitWorld(&TestWorld, &MainAllocatorSystem, 2);
		
		EntityID player = CreateEntity(&TestWorld, "kai");
		SpriteRenderer sr = { CreateSprite(vec2(50, 50), vec3(0, 0, 0)), Material{ SetTexture("resources\\textures\\tile2.png"), Color(1, 1, 1, 1) } };
		RegisterSpriteRenderer(&TestWorld, player, sr);

		EntityID player1 = CreateEntity(&TestWorld, "kai1");
		SpriteRenderer sr1 = { CreateSprite(vec2(50, 50), vec3(50, 0, 0)), Material{ SetTexture("resources\\textures\\tile1.png"), Color(1, 1, 1, 1) } };
		RegisterSpriteRenderer(&TestWorld, player1, sr1);
	}

	{
		shader.Activate();

		glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projectionMatrix"), 1, false, OrthoProjectionMatrix(&Cam, screenWidth, screenHeight, -0.1f, 500.0f).GetElemets());
		glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "viewMatrix"), 1, false, LookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0)).GetElemets());
	}
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	shader.Activate();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, ModelMatrix(&transform).GetElemets());

	RenderSpriteRendererSystem(&TestWorld);
}

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	
}

GAME_DLL GAME_SHUTDOWN(Game_Shutdown)
{
	ShutDownMemorySystem(&MainAllocatorSystem);
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

void InitCamera()
{
	OrthoProjectionMatrix(&Cam, screenWidth, screenHeight, -0.1f, 500.0f);
	//PerspectiveProjection(&Cam, 80.0f, screenWidth, screenHeight, -0.1f, 500.0f);

	LookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));
}