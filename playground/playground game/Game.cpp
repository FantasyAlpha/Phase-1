#include "Game.h"

#define ORTHO 1

global_variable Game_Resources Resources;

float screenWidth;
float screenHeight;

global_variable MainAllocator MainAllocatorSystem;
global_variable StackAllocator WorldStack;
global_variable World Game_World;

//Movement

AutomaticDirections dir;
void AutomaticMove(char *ActorName, float velocity,float delta)
{
	if (dir.Rigth)
	{
		
		if (! Game_World.CollisionManager.GetCollider(ActorName)->rigth )
		{
			Game_World.TransformManager.GetTransform(ActorName)->Position.x += velocity*0.016f*40.0f;
			float temp = Game_World.TransformManager.GetTransform(ActorName)->Position.x;
			Game_World.CollisionManager.GetCollider(ActorName)->pos->x = temp;

			//Movable->collider.pos.x += velocity*0.016f*40.0f;
			Game_World.CollisionManager.GetCollider(ActorName)->velocity.x = velocity*0.016f*40.0f;

			dir.counter++;
			if (dir.counter > velocity*100.0f)
			{
				dir.Rigth = false;
				dir.Left = true;
				dir.counter = 0;
			}
		}
		else
		{
			dir.Rigth = false;
			dir.Left = true;
			dir.counter = 0;

		}
	}


	if (dir.Left)
	{
		if ( !Game_World.CollisionManager.GetCollider(ActorName)->left)
		{
			Game_World.TransformManager.GetTransform(ActorName)->Position.x -= velocity*0.016f*40.0f;
		//	Movable->collider.pos.x -= velocity*0.016f*40.0f;
			float temp = Game_World.TransformManager.GetTransform(ActorName)->Position.x;
		    Game_World.CollisionManager.GetCollider(ActorName)->pos->x = temp;

			Game_World.CollisionManager.GetCollider(ActorName)->velocity.x = -velocity*0.016f*40.0f;

			dir.counter++;
			if (dir.counter > velocity*100.0f)
			{
				dir.Rigth = true;
				dir.Left = false;
				dir.counter = 0;
			}

		}
		else {
			dir.Rigth = true;
			dir.Left = false;
			dir.counter = 0;

		}
	}
}
float Accelerate(float flGoal, float flCurrent, float delta){


	float flDifference = flGoal - abs(flCurrent);

	if (flDifference > delta)

		// vdash= v+at
		// new velocity= current velocity + delta * acceleration constant 

		return abs(flCurrent) + delta * 60.0f;

	if (flDifference < -delta)
		return abs(flCurrent) - delta*60.0f;

	return flGoal;
}

vec2 currentvelocity=0.0f;
void Move(char *ActorName, vec2 goalVelocity, float delta, Game_Input *input, bool Run)
{

	currentvelocity = Game_World.CollisionManager.GetCollider(ActorName)->velocity;

	if (!input->LEFT.KeyDown&&!input->RIGHT.KeyDown){
		Game_World.CollisionManager.GetCollider(ActorName)->velocity.x = 0.0f;

		currentvelocity.x = 0.0f;

	}
	// if (input->RIGHT.KeyUp){
	//	//Game_World.CollisionManager.GetCollider(ActorName)->velocity.x = 0.0f; 
	//	currentvelocity.x = 0.0f;

	//}

	else if (input->RIGHT.KeyDown)
	{
		if (Game_World.CollisionManager.GetCollider(ActorName)->rigth == false)
		{
			currentvelocity.x = Accelerate(goalVelocity.x, currentvelocity.x, delta);

			Game_World.TransformManager.GetTransform(ActorName)->Position.x += currentvelocity.x;
			float temp = Game_World.TransformManager.GetTransform(ActorName)->Position.x;
			Game_World.CollisionManager.GetCollider(ActorName)->pos->x = temp;

			Game_World.CollisionManager.GetCollider(ActorName)->velocity = currentvelocity;

		}


	}
	else if (input->LEFT.KeyDown)
	{
		if (Game_World.CollisionManager.GetCollider(ActorName)->left == false)
		{
			currentvelocity.x = -1.0f * Accelerate(goalVelocity.x, currentvelocity.x, delta);

			Game_World.TransformManager.GetTransform(ActorName)->Position.x += currentvelocity.x;
			float temp = Game_World.TransformManager.GetTransform(ActorName)->Position.x;
			Game_World.CollisionManager.GetCollider(ActorName)->pos->x = temp;

			Game_World.CollisionManager.GetCollider(ActorName)->velocity = currentvelocity;

		}
	}
	

	std::cout << "player velocity " << currentvelocity.x << std::endl;

}



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
//Game_Input *input
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

	/*if (input->DOWN.KeyDown)
	{
		Game_World.TransformManager.GetTransform("player")->Position.x += 100.0f*delta;
		Game_World.CollisionManager.GetCollider("player")->velocity = 100.0f*delta;


	}*/

	AutomaticMove("brick", 2.0f,delta);


	Move("player",5.0f, delta, input, false);

	Game_World.UpdateWorld(delta);
}

GAME_DLL GAME_SHUTDOWN(Game_Shutdown)
{
}

void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

void InitResources()
{
	// add resources : Textures
	//AddTexture(&Resources, LoadTexture("resources\\textures\\walk1.png"), "SHEET");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile1.png"), "Tile1");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile2.png"), "Tile2");
	AddTexture(&Resources, LoadTexture("resources\\textures\\empty.png"), "Empty");
	AddTexture(&Resources, LoadTexture("resources\\textures\\player_2.png"), "PLAYER");
	AddTexture(&Resources, LoadTexture("resources\\textures\\Neo In.png"), "neo");

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
		// Actor creations

      /*  Game_World.ActorManager.CreateActor("KAI");
		Game_World.ActorManager.CreateActor("KAI1");*/

		Game_World.ActorManager.CreateActor("KAI2");
		Game_World.ActorManager.CreateActor("brick");
		Game_World.ActorManager.CreateActor("player");
	}

	{
		// Transform components

      /*   Game_World.TransformManager.AddComponent("KAI", TransformComponent{});
		Game_World.TransformManager.AddComponent("KAI1", TransformComponent{});*/

		Game_World.TransformManager.AddComponent("KAI2", TransformComponent{});
		Game_World.TransformManager.GetTransform("KAI2")->Position = vec3f(-300.0f, 0, 
			Game_World.TransformManager.GetTransform("KAI2")->Position.z);

		Game_World.TransformManager.AddComponent("brick", TransformComponent{});
		Game_World.TransformManager.GetTransform("brick")->Position = vec3f(120.0f, -100.0f,
			Game_World.TransformManager.GetTransform("brick")->Position.z);

		Game_World.TransformManager.AddComponent("player", TransformComponent{});
		Game_World.TransformManager.GetTransform("player")->Position = vec3f(0.0f, -100.0f,
		 Game_World.TransformManager.GetTransform("player")->Position.z);


	}

	{
		// Animation components

		uint32 frames[4];
		
		frames[0] = 4;
		frames[1] = 5;
		frames[2] = 6;
		frames[3] = 7;

		Game_World.AnimationManager.AddComponent("a1", 4, 2, frames, 4, 0.001f, true);
		

		/*uint32 frames[48];
		for (uint32 i = 0; i < 48; i++)
		{
			frames[i] = i;
		}
		Game_World.AnimationManager.AddComponent("a1", 3, 16, frames, 48, 0.1f, true);*/
		
	}

	{
		// Render components

		
	/*	Game_World.RendererManager.AddComponent("KAI"
			, vec3f(-100.0f, -100.0f, 0), vec2(100, 100)
			, Material{ GetTexture(&Resources, "PLAYER"), Color{ 1, 1, 1, 1 } }, Game_World.AnimationManager.GetAnimationClip("a1"));

		Game_World.RendererManager.AddComponent("KAI1"
			, vec3f(0.0f, 200.0f, 0), vec2(200.0f, 100.0f)
			, Material{ GetTexture(&Resources, "Tile1"), Color{ 0, 0.3f, 0.8f, 1 } });
			*/
		Game_World.RendererManager.AddComponent("KAI2"
			, Game_World.TransformManager.GetTransform("KAI2")->Position, vec2(100.0f, 100.0f)
			, Material{ GetTexture(&Resources, "Empty"), Color{ 0, 0.3f, 0.8f, 1 } });

		Game_World.RendererManager.AddComponent("brick"
			, Game_World.TransformManager.GetTransform("brick")->Position, vec2(100.0f, 50.0f)
			, Material{ GetTexture(&Resources, "Empty"), Color{ 1, .3f, .5f, 1 } });

		Game_World.RendererManager.AddComponent("player"
			, Game_World.TransformManager.GetTransform("player")->Position, vec2(100.0f, 200.0f)
			, Material{ GetTexture(&Resources, "neo"), Color{ 1, 1, 1, 1 } });

	}
	{
		//Collision component

		// Addcomponent (actorName , vec3f pos , vec2 size , vec2 velocity ,bool wall ,bool trigger)

		Game_World.CollisionManager.AddComponent("KAI2", &Game_World.TransformManager.GetTransform("KAI2")->Position, 
			vec2(100.0f, 100.0f),vec2(0.0f,0.0f),false,false );
		Game_World.CollisionManager.AddComponent("brick", &Game_World.TransformManager.GetTransform("brick")->Position,
			vec2(100.0f, 50.0f), vec2(0.0f, 0.0f), false, false);
		Game_World.CollisionManager.AddComponent("player", &Game_World.TransformManager.GetTransform("player")->Position, 
			vec2(100.0f, 200.0f),vec2(0.0f,0.0f),false,false);

	}

	{

		// create parent / child transform 

		/*Game_World.TransformManager.AttachTransformChild("KAI", "KAI1");
		Game_World.TransformManager.AttachTransformChild("KAI1", "KAI2");*/

		Game_World.TransformManager.AttachTransformChild("KAI2", "player");
	}
}




