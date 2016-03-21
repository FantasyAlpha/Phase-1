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

	{
		InitMainMemorySystem(&MainAllocatorSystem, AllocatorTypes::STACK_ALLOCATOR, Megabytes(500), 4, Megabytes(100));
	}

	{
		Game_World.InitWorld(MainAllocatorSystem.StackSystem);
		Game_World.RendererManager.InitMainShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
	}	

	{
		Game_World.ActorManager.CreateActor("KAI");
		Game_World.ActorManager.CreateActor("KAI1");
		Game_World.ActorManager.CreateActor("KAI2");
	}

	{
		Game_World.RendererManager.AddComponent("KAI"
			, Renderer{ CreateSprite(vec3(0, 0, 0), vec2(50, 50))
			, Material{ GetTexture(&Resources, "Tile1"), Color(1, 1, 1, 1) } });

		Game_World.RendererManager.AddComponent("KAI1"
			, Renderer{ CreateSprite(vec3(0, 80, 0), vec2(50, 50))
			, Material{ GetTexture(&Resources, "Tile2"), Color(1, 1, 1, 1) } });

		Game_World.RendererManager.AddComponent("KAI2"
			, Renderer{ CreateSprite(vec3(80, 80, 0), vec2(50, 50))
			, Material{ GetTexture(&Resources, "Tile1"), Color(1, 1, 1, 1) } });
	}

	{
		Game_World.TransformManager.AddComponent("KAI", TransformComponent{});
		Game_World.TransformManager.AddComponent("KAI1", TransformComponent{});
		Game_World.TransformManager.AddComponent("KAI2", TransformComponent{});
	}

	{
		Game_World.TransformManager.AttachTransformChild("KAI", "KAI1");
		Game_World.TransformManager.AttachTransformChild("KAI1", "KAI2");
	}
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	Game_World.RenderWorld();
}

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	if (input->RIGHT.KeyDown)
	{
		Game_World.TransformManager.GetTransform("KAI1")->Position.x += 1;
	}

	if (input->LEFT.KeyDown)
	{
		Game_World.TransformManager.GetTransform("KAI")->Position.x -= 1;
	}

	Game_World.UpdateWorld();
}

GAME_DLL GAME_SHUTDOWN(Game_Shutdown)
{
	ShutDownMemorySystem(&MainAllocatorSystem);
}

void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

void InitResources()
{
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile1.png"), "Tile1");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile2.png"), "Tile2");
}