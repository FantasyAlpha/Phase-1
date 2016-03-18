#include "Game.h"

global_variable Game_Resources Resources;

global_variable Mesh TestMesh;
global_variable Material TestMaterial;
global_variable Shader shader;

global_variable Transform transform;
global_variable Camera Cam;

float screenWidth;
float screenHeight;

global_variable MainAllocator MainAllocatorSystem;
global_variable StackAllocator WorldStack;
global_variable World AM;

//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	{
		screenWidth = dimensions.Width;
		screenHeight = dimensions.Height;
	}

	{
		InitShaders();
		InitResources();
	}

	{
		InitMainMemorySystem(&MainAllocatorSystem, AllocatorTypes::STACK_ALLOCATOR, 500 * 1024 * 1024, 4);
	}

	{
		InitPartialStackSystem(MainAllocatorSystem.StackSystem, &WorldStack, TOTAL_SPRITES_SIZE + TOTAL_TRANSFORMS_SIZE + TOTAL_ACTORS_SIZE);
		InitWorld(&WorldStack, &AM);
		InitSpriteRendererSystem(&WorldStack, &AM);
		InitMainShader(&AM, "resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
		InitTransformSystem(&WorldStack, &AM);
	}

	

	{
		CreateActor(&AM, "KAI");
		CreateActor(&AM, "KAI1");
		CreateActor(&AM, "KAI2");
		CreateActor(&AM, "KAI3");
		CreateActor(&AM, "KAI4");
		AddRenderer(&AM, "KAI", SpriteRenderer{ CreateSprite(vec2(50, 50), vec3(0, 0, 0)), Material{ GetTexture(&Resources, "Tile1"), Color(1, 1, 1, 1) } });
		AddRenderer(&AM, "KAI1", SpriteRenderer{ CreateSprite(vec2(50, 50), vec3(0, 80, 0)), Material{ GetTexture(&Resources, "Tile2"), Color(1, 1, 1, 1) } });
		AddRenderer(&AM, "KAI3", SpriteRenderer{ CreateSprite(vec2(50, 50), vec3(80, 80, 0)), Material{ GetTexture(&Resources, "Tile1"), Color(1, 1, 1, 1) } });
	}

	{
		AddTransform(&AM, "KAI", TransformComponent{});
		AddTransform(&AM, "KAI1", TransformComponent{});
		AddTransform(&AM, "KAI2", TransformComponent{});
		AddTransform(&AM, "KAI3", TransformComponent{});
		AddTransform(&AM, "KAI4", TransformComponent{});
		AddTransformChild(&AM, "KAI", "KAI1");
		AddTransformChild(&AM, "KAI", "KAI2");
		AddTransformChild(&AM, "KAI1", "KAI3");
		AddTransformChild(&AM, "KAI1", "KAI4");
		SetMainParent(&AM, "KAI");
	//	RemoveTransform(&AM, "KAI1");
	}

	{
		ActivateShader(&shader);

		glUniformMatrix4fv(GetUniformLocation(&shader, UNIFORMS::PROJECTION_MATRIX), 1, false, OrthoProjectionMatrix(&Cam, screenWidth, screenHeight, -0.1f, 500.0f).GetElemets());
		glUniformMatrix4fv(GetUniformLocation(&shader, UNIFORMS::VIEW_MATRIX), 1, false, LookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0)).GetElemets());
	}
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	RendererSprites(&AM);
}

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	if (input->RIGHT.KeyDown)
	{
		GetTransform(&AM, "KAI1")->Position.x += 1;
	}

	if (input->LEFT.KeyDown)
	{
		GetTransform(&AM, "KAI")->Position.x -= 1;
	}

	UpdateTransformSystem(&AM);
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
	shader = CreateShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	shader = CreateShader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");
#endif
}

void InitCamera()
{
	OrthoProjectionMatrix(&Cam, screenWidth, screenHeight, -0.1f, 500.0f);
	//PerspectiveProjection(&Cam, 80.0f, screenWidth, screenHeight, -0.1f, 500.0f);

	LookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));
}

void InitResources()
{
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile1.png"), "Tile1");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile2.png"), "Tile2");
}