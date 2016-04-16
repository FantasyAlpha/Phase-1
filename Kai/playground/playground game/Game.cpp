#include "Game.h"

#define ORTHO 0

global_variable Game_Resources Resources;

float screenWidth;
float screenHeight;

global_variable Transform trans1;
global_variable Transform trans2;
global_variable Transform trans3;
global_variable Transform trans4;
global_variable Camera cam;
global_variable Shader shader;

global_variable Renderer r1;
global_variable Renderer r2;
global_variable Renderer r3;
global_variable Renderer r4;

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
	InitEverything();
	//InitGameWorld();
}

//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	/*
	*/
	//RenderSingleRenderer(&r1, &shader, &trans1, &cam);
	//RenderSingleRenderer(&r2, &shader, &trans2, &cam);
	RenderSingleRenderer(&r3, &shader, &trans3, &cam);
	RenderSingleRenderer(&r4, &shader, &trans4, &cam);
	//Game_World.RenderWorld();
}

float speed = 0.0f;

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
	speed = 0.5f;
	trans3.Rotation.x += speed;//sinf(speed) * (1.0f / 60.0f); 
	//trans3.Position = vec3(0.0f);

	if (input->RIGHT.KeyDown)
	{
		cam.Eye.x += 0.1f;
		//cam.Target.x -= 0.01f;// vec3() - cam.Target;
		//Game_World.TransformManager.GetTransform("KAI1")->Position.x += 1;
	}

	if (input->LEFT.KeyDown)
	{
		//Game_World.TransformManager.GetTransform("KAI")->Position.x -= 1;
		cam.Eye.x -= 0.1f;
		//cam.Target.x += 0.01f;// vec3() - cam.Target;

	}

	if (input->UP.KeyDown)
	{
		trans3.Position.z += 1;
	}

	if (input->DOWN.KeyDown)
	{
		trans3.Position.z -= 1;
	}

	if (input->W.KeyDown)
	{
		trans4.Position.z -= 1;
	}
	if (input->S.KeyDown)
	{
		trans4.Position.z += 1;
	}
	if (input->A.KeyDown)
	{
		trans4.Position.x -= 1;
	}
	if (input->D.KeyDown)
	{
		trans4.Position.x += 1;
	}

	//Game_World.UpdateWorld();
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
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile1.png"), "Tile1");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile2.png"), "Tile2");
	AddTexture(&Resources, LoadTexture("resources\\textures\\empty.png"), "Empty");
}

void InitGameWorld()
{
	{
		InitMainMemorySystem(&MainAllocatorSystem, AllocatorTypes::STACK_ALLOCATOR, Megabytes(500), 4);
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
			, Material{ GetTexture(&Resources, "Tile1"), Color{ 1, 1, 1, 1 } } });

		Game_World.RendererManager.AddComponent("KAI1"
			, Renderer{ CreateSprite(vec3(0, 80, 0), vec2(50, 50))
			, Material{ GetTexture(&Resources, "Tile2"), Color{ 1, 1, 1, 1 } } });

		Game_World.RendererManager.AddComponent("KAI2"
			, Renderer{ CreateSprite(vec3(80, 80, 0), vec2(50, 50))
			, Material{ GetTexture(&Resources, "Tile1"), Color{ 1, 1, 1, 1 } } });
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

void InitEverything()
{
	shader = CreateShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
	
	AddUniform(&shader, UNIFORMS::COLOR, "myColor");

	AddUniform(&shader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");
	AddUniform(&shader, UNIFORMS::VIEW_MATRIX, "viewMatrix");
	AddUniform(&shader, UNIFORMS::MODEL_MATRIX, "modelMatrix");
	AddUniform(&shader, UNIFORMS::AMBIENT_COLOR, "ambientColor");
	AddUniform(&shader, UNIFORMS::AMBIENT_STRENGTH, "ambientStrength");

	cam = Camera(vec2(screenWidth, screenHeight));
	//cam.Size = vec2(screenWidth, screenHeight);
	cam.Type = CameraType::PERSPECTIVE;

#if ORTHO == 1
	float spriteSize = 50;
	float cubeSize = 50;
#else
	float spriteSize = 1;
	float cubeSize = 1;
#endif

	r1 = Renderer{ CreateSprite(trans1.Position, vec2(spriteSize, spriteSize))
		, Material{ GetTexture(&Resources, "Empty"), Color{ 1, 1, 1, 1 } } };

	r2 = Renderer{ CreateSprite(trans2.Position, vec2(spriteSize, spriteSize))
		, Material{ GetTexture(&Resources, "Empty"), Color{ 1, 0.3, 1, 1 } } };

	r3 = Renderer{ CreateCube(trans3.Position, vec3(cubeSize, cubeSize, cubeSize))
		, Material{ GetTexture(&Resources, "Empty"), Color{ 0, 0.5f, 1, 1 } } };
	trans3.Position = vec3(0, 0, -8.0f);
	
	r4 = Renderer{ CreateCube(trans4.Position, vec3(cubeSize, cubeSize, cubeSize))
		, Material{ GetTexture(&Resources, "Empty"), Color{ 1, 1.0f, 1, 1 } } };
	trans4.Position = vec3(cubeSize * 2.0f, 0, -8.0f);

}