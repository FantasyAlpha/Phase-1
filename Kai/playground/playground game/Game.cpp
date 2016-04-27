#include "Game.h"

#define ORTHO 1

global_variable Game_Resources Resources;

float screenWidth;
float screenHeight;

global_variable MainAllocator MainAllocatorSystem;
global_variable StackAllocator WorldStack;
global_variable World Game_World;

//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	{
		screenWidth = dimensions.Width;
		screenHeight = dimensions.Height;
	}

	{
		InitResources();
	}
	InitGameWorld();
}
float x, y;
bool debug = false;
//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{	
	Game_World.RenderWorld(debug);
}

float speed = 0.0f;

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	Game_World.MousePos.x = input->MousePos.x - (screenWidth / 2.0f);
	Game_World.MousePos.y = -input->MousePos.y + (screenHeight / 2.0f);

	if (input->A.KeyDown)
	{
		debug = true;
	}

	if (input->D.KeyDown)
	{
		debug = false;
	}

	Game_World.UpdateWorld();
}

GAME_DLL GAME_SHUTDOWN(Game_Shutdown)
{
//	ShutDownMemorySystem(&MainAllocatorSystem);
}

void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

void InitResources()
{
	//AddTexture(&Resources, LoadTexture("resources\\textures\\walk1.png"), "SHEET");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile1.png"), "Tile1");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile2.png"), "Tile2");
	AddTexture(&Resources, LoadTexture("resources\\textures\\empty.png"), "Empty");
	AddTexture(&Resources, LoadTexture("resources\\textures\\player_2.png"), "PLAYER");
}

void InitGameWorld()
{
	{}

	{
		InitMainMemorySystem(&MainAllocatorSystem, AllocatorTypes::STACK_ALLOCATOR, Megabytes(500), 4);
		Game_World.MainCamera = Camera(vec2(screenWidth, screenHeight));
		Game_World.MainCamera.Type = CameraType::ORTHOGRAPHIC;
	}

	{
		Game_World.InitWorld(MainAllocatorSystem.StackSystem);
		Game_World.RendererManager.InitMainShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
		Game_World.RendererManager.InitDebugShader("resources\\shaders\\vertex shader 120_2.vert", "resources\\shaders\\fragment shader 120_2.frag");
	}

	{
		Game_World.ActorManager.CreateActor("KAI");
		Game_World.ActorManager.CreateActor("KAI1");
		Game_World.ActorManager.CreateActor("KAI2");
	}

	{
		Game_World.TransformManager.AddComponent("KAI", TransformComponent{});
		Game_World.TransformManager.AddComponent("KAI1", TransformComponent{});
		Game_World.TransformManager.AddComponent("KAI2", TransformComponent{});
	}

	{
		uint32 frames[4];
		
		frames[0] = 4;
		frames[1] = 5;
		frames[2] = 6;
		frames[3] = 7;

		Game_World.AnimationManager.AddComponent("a1", 4, 2, frames, 4, 0.1f, true);
		/*

		uint32 frames[48];
		for (uint32 i = 0; i < 48; i++)
		{
			frames[i] = i;
		}
		Game_World.AnimationManager.AddComponent("a1", 3, 16, frames, 48, 0.1f, true);
		*/
	}

	{

		Game_World.RendererManager.AddComponent("KAI"
			, vec3(0, 0, 0), vec2(100, 100)
			, Material{ GetTexture(&Resources, "PLAYER"), Color{ 0, 1, 1, 1 } }, Game_World.AnimationManager.GetAnimationClip("a1"));

		Game_World.RendererManager.AddComponent("KAI1"
			, vec3(0, 200, 0), vec2(200, 100)
			, Material{ GetTexture(&Resources, "Tile1"), Color{ 0, 0.3f, 0.8f, 1 } });

		Game_World.RendererManager.AddComponent("KAI2"
			, vec3(80, 80, 0), vec2(50, 50)
			, Material{ GetTexture(&Resources, "Empty"), Color{ 0, 0.3f, 0.8f, 1 } });
	}

	{
		Game_World.TransformManager.AttachTransformChild("KAI", "KAI1");
		Game_World.TransformManager.AttachTransformChild("KAI1", "KAI2");
	}
}

void InitEverything()
{
//	shader = CreateShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
//	
//	//AddUniform(&shader, UNIFORMS::COLOR, "myColor");
//
//	AddUniform(&shader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");
//	AddUniform(&shader, UNIFORMS::VIEW_MATRIX, "viewMatrix");
//	AddUniform(&shader, UNIFORMS::MODEL_MATRIX, "modelMatrix");
//	AddUniform(&shader, UNIFORMS::AMBIENT_COLOR, "ambientColor");
//	AddUniform(&shader, UNIFORMS::AMBIENT_STRENGTH, "ambientStrength");
//
//	cam = Camera(vec2(screenWidth, screenHeight));
//	//cam.Size = vec2(screenWidth, screenHeight);
//
//#if ORTHO == 1
//	float spriteSize = 50;
//	float cubeSize = 50;
//	cam.Type = CameraType::ORTHOGRAPHIC;
//#else
//	float spriteSize = 1;
//	float cubeSize = 1;
//	cam.Type = CameraType::PERSPECTIVE;
//
//#endif

}