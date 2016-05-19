#include "Game.h"

global_variable Game_Resources Resources;

float screenWidth;
float screenHeight;
float UPS;

global_variable SceneManager Game_Scene;

//Movement

AutomaticDirections dir;
void AutomaticMove(char *ActorName, float velocity, float delta)
{
	// if collide change your direction 
	if (dir.Rigth)
	{
		
		if (!Game_Scene.CollisionManager.GetCollider(ActorName)->rigth)
		{
			Game_Scene.ActorManager.GetTransform(ActorName)->Position.X += velocity*delta*40.0f;
			/*float temp = Game_Scene.ActorManager.GetTransform(ActorName)->Position.X;
			Game_Scene.CollisionManager.GetCollider(ActorName)->pos->X = temp;*/

			//Movable->collider.pos.X += velocity*0.016f*40.0f;
			Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.X = velocity*delta*40.0f;

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
		if (!Game_Scene.CollisionManager.GetCollider(ActorName)->left)
		{
			Game_Scene.ActorManager.GetTransform(ActorName)->Position.X -= velocity*delta*40.0f;
			//	Movable->collider.pos.X -= velocity*0.016f*40.0f;
			/*float temp = Game_Scene.ActorManager.GetTransform(ActorName)->Position.X;
			Game_Scene.CollisionManager.GetCollider(ActorName)->pos->X = temp;*/

			Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.X = -velocity*delta*40.0f;

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
float Accelerate(float flGoal, float flCurrent, float delta)
{
	float flDifference = flGoal - abs(flCurrent);

	if (flDifference > delta)

		// vdash= v+at
		// new velocity= current velocity + delta * acceleration constant 

		return abs(flCurrent) + delta * Game_Scene.CollisionManager.physics.acceleration;

	if (flDifference < -delta)
		return abs(flCurrent) - delta* Game_Scene.CollisionManager.physics.acceleration;

	return flGoal;
}

vec2f currentvelocity=0.0f;
void Move(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input, bool Run)
{
	currentvelocity.X = Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.X;

	if ((input->RIGHT.KeyUp) && (input->LEFT.KeyUp)/*!input->LEFT.KeyDown&&!input->RIGHT.KeyDown*/){
		Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.X = 0.0f;

		currentvelocity.X = 0.0f;
	}
	else if (input->RIGHT.KeyDown)
	{
		if (Game_Scene.CollisionManager.GetCollider(ActorName)->rigth == false)
		{
			//currentvelocity.X = Accelerate(goalVelocity.X, currentvelocity.X, delta);
			currentvelocity.X = goalVelocity.X*delta;

			Game_Scene.ActorManager.GetTransform(ActorName)->Position.X += currentvelocity.X;
			Game_Scene.ActorManager.GetTransform(ActorName)->Scale.X = 1;
			Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.X = currentvelocity.X;
		}
	}
	else if (input->LEFT.KeyDown)
	{
		if (Game_Scene.CollisionManager.GetCollider(ActorName)->left == false)
		{
			//currentvelocity.X = -1.0f * Accelerate(goalVelocity.X, currentvelocity.X, delta);

			currentvelocity.X = -1.0f*goalVelocity.X*delta;

			Game_Scene.ActorManager.GetTransform(ActorName)->Position.X += currentvelocity.X;

			Game_Scene.ActorManager.GetTransform(ActorName)->Scale.X = -1;

			Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.X = currentvelocity.X;
		}
	}	

	AutoMoveCameraHorizontal(&Game_Scene.MainCamera, currentvelocity, Game_Scene.ActorManager.GetTransform(ActorName)->Position, Game_Scene.CollisionManager.GetCollider(ActorName)->size);
	AutoMoveCameraVertical(&Game_Scene.MainCamera, currentvelocity, Game_Scene.ActorManager.GetTransform(ActorName)->Position, Game_Scene.CollisionManager.GetCollider(ActorName)->size);

	//std::cout << "player velocity " << currentvelocity.X << std::endl;
}

float fallSpeed = 0.0f;
bool jumpflag = false;
//float Gravity = 2.0f;
float jumpSpeed = 20;
void jumpHandle(char*ActorName, Game_Input *input, float delta)
{

	bool groundCheck = false;
	bool Jump;

	Jump = Game_Scene.CollisionManager.GetCollider(ActorName)->jump;

	if (Jump)
	{
		groundCheck = true;
	}

	if (groundCheck)
	{
		//Jump = true;
		fallSpeed = 0;
		Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.Y = 0;
	}
	if (input->Space.KeyDown&&groundCheck)
	{
		// reset 
		jumpSpeed = 20;

		jumpflag = true;
		Jump = false;
		groundCheck = false;

	}

	if (Game_Scene.CollisionManager.GetCollider(ActorName)->up)
	{
		jumpflag = false;
	}

	if (jumpflag&&fallSpeed == 0){
		Game_Scene.ActorManager.GetTransform(ActorName)->Position.Y += jumpSpeed;
		//	Game_Scene.CollisionManager.GetCollider(ActorName)->pos->Y += jumpSpeed;
		Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.Y = jumpSpeed;

		jumpSpeed -= Game_Scene.CollisionManager.physics.Gravity*delta * 10.0f;
		if (jumpSpeed <= 0){
			jumpflag = false;
			// Reset jump speed ****************importatnt
			jumpSpeed = 20;

		}
	}
	else if (!groundCheck)
	{

		fallSpeed += (Game_Scene.CollisionManager.physics.Gravity*delta*25.0f);

		Game_Scene.ActorManager.GetTransform(ActorName)->Position.Y -= fallSpeed;
		//Game_Scene.CollisionManager.GetCollider(ActorName)->pos->Y -= fallSpeed;
		Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.Y = fallSpeed;

	}
}


GAME_DLL GAME_INIT(Game_Init)
{
	{
		screenWidth = dimensions.Width;
		screenHeight = dimensions.Height;
		UPS = ups;
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
	Game_Scene.RenderScene(debug);
}

//Update the game
//Game_Input *input
GAME_DLL GAME_UPDATE(Game_Update)
{
	Game_Scene.MousePos.X = input->MousePos.X - (screenWidth / 2.0f);
	Game_Scene.MousePos.Y = -input->MousePos.Y + (screenHeight / 2.0f);

	if (input->P.KeyDown)
	{
		debug = true;
	}
	if (input->O .KeyDown)
	{
		debug = false;
	}
	
	if (input->W.KeyDown)
	{
		Game_Scene.RendererManager.AmbientStrength += 2;
	}

	if (input->S.KeyDown)
	{
		Game_Scene.RendererManager.AmbientStrength -= 2;
	}

	if (input->D.KeyDown)
	{
		Game_Scene.ActorManager.DestroyActor("KAI2");
		
	}

	if (input->A.KeyDown)
	{
		Game_Scene.ActorManager.AddActor("KAI2", Transform{});
		Game_Scene.ActorManager.GetTransform("KAI2")->Position = vec3f(-200.0f, 200.0f, 0);
	Game_Scene.RendererManager.AddComponent("KAI2"
			, RenderableType::Movable
			, vec2f(50.0f, 50.0f)
			, Material{ GetTexture(&Resources, "star"), vec4f{ 1, 1, 1, 1 } });
		Game_Scene.CollisionManager.AddComponent("KAI2", &Game_Scene.ActorManager.GetTransform("KAI2")->Position,
			Game_Scene.RendererManager.GetRenderable("KAI2", RenderableType::Movable)->Size, false, false, false);
	}
	

	//Game_Scene.ActorManager.GetTransform("KAI2")->Rotation.Z += 1;
	
	AutomaticMove("brick", 2.0f, delta);

	Game_Scene.CollisionManager.CheckGroundCollision("player");

	jumpHandle("player", input, 0.016f);
	Move("player", 400.0f, delta, input, false);

	Game_Scene.UpdateScene(0.016f);

}

GAME_DLL GAME_SHUTDOWN(Game_Shutdown)
{
}

void InitResources()
{
	/*
	//Initialize SDL AUDIO
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: %s\n", SDL_GetError();
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: \n";
	}
	*/

	// add resources : Textures
	//AddTexture(&Resources, LoadTexture("resources\\textures\\noe walk.png"), "noe walk");
	AddTexture(&Resources, LoadTexture("resources\\textures\\noe walk.png"), "noe idle");
	AddTexture(&Resources, LoadTexture("resources\\textures\\empty.png"), "Empty");
	//AddTexture(&Resources, LoadTexture("resources\\textures\\player_2.png"), "PLAYER");
	AddTexture(&Resources, LoadTexture("resources\\textures\\Neo In.png"), "neo");
	AddTexture(&Resources, LoadTexture("resources\\textures\\back.png"), "Back");
	AddTexture(&Resources, LoadTexture("resources\\textures\\star.png"), "star");
	AddTexture(&Resources, LoadTexture("resources\\textures\\grayCube.png"), "cube");


	//AddBGM(&Resources, "resources\\audio\\testClip.wav", "benny");
}

void InitGameWorld()
{
	{
		Game_Scene.MainCamera = Camera(vec2f(screenWidth, screenHeight));
		Game_Scene.MainCamera.Type = CameraType::ORTHOGRAPHIC;
		Game_Scene.MainCamera.WindowSize = Game_Scene.MainCamera.Size / 2.0f;
	}

	{
		Game_Scene.InitScene(100);
		Game_Scene.Delta = 1.0f / UPS;
	}

	{
		// Actor creations

		Game_Scene.ActorManager.AddActor("KAI2", Transform{});
		Game_Scene.ActorManager.AddActor("backGround", Transform{});
		Game_Scene.ActorManager.AddActor("player", Transform{});
		Game_Scene.ActorManager.AddActor("platform", Transform{});
		Game_Scene.ActorManager.AddActor("brick", Transform{});
		Game_Scene.ActorManager.AddActor("wall", Transform{});

	}

	{
		// Modify Transforms

		Game_Scene.ActorManager.GetTransform("backGround")->Position = vec3f(0, 0, 0);

		Game_Scene.ActorManager.GetTransform("platform")->Position = vec3f(0, -330.0f, 0);

		Game_Scene.ActorManager.GetTransform("KAI2")->Position = vec3f(-200.0f, 200.0f, 0);
		
		Game_Scene.ActorManager.GetTransform("brick")->Position = vec3f(0.0f, -290.0f, 0);

		//Game_Scene.ActorManager.GetTransform("wall")->Position = vec3f(500.0f, 0.0f, 0);

		Game_Scene.ActorManager.GetTransform("player")->Position = vec3f(200.0f, -200.0f, 0);

		//Game_Scene.ActorManager.AttachChild("player", "KAI2");

	}

	{
		// Animation components

		uint32 Walkframes[10];
		
		Walkframes[0] = 0;
		Walkframes[1] = 1;
		Walkframes[2] = 2;
		Walkframes[3] = 3;
		Walkframes[4] = 4;
		Walkframes[5] = 5;
		Walkframes[6] = 6;
		Walkframes[7] = 7;
		Walkframes[8] = 8;
		Walkframes[9] = 
			9;

		Game_Scene.AnimationManager.AddComponent("noe idle clip", 1, 10, Walkframes, 10, 10, true);
		
	}

	{
		// Render components
//Game_Scene.AnimationManager.GetAnimationClip("a1")
		Game_Scene.RendererManager.InitMainShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
		Game_Scene.RendererManager.InitDebugShader("resources\\shaders\\vertex shader 120_2.vert", "resources\\shaders\\fragment shader 120_2.frag");

		/*
			*/
		Game_Scene.RendererManager.AddComponent("backGround"
			, RenderableType::Static
			, vec2f(screenWidth * 2.0f, screenHeight)
			, Material{ GetTexture(&Resources, "Back"), vec4f{ 1, 1, 1, 1 } });
		
		Game_Scene.RendererManager.AddComponent("brick"
			, RenderableType::Movable
			, vec2f(100.0f, 50.0f)
			, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, .3f, .5f, 1 } });

		/*
		Game_Scene.RendererManager.AddComponent("wall"
			, RenderableType::Static
			, vec2f(100, 50)
			, Material{ GetTexture(&Resources, "Tile2"), vec4f{ 1, 1, 1, 1 } });
		*/
		
		Game_Scene.RendererManager.AddComponent("player"
			, RenderableType::Movable
			, vec2f(200, 266)
			, Material{ GetTexture(&Resources, "neo"), vec4f{ 1, 1, 1, 1 } });

		Game_Scene.RendererManager.AddComponent("KAI2"
			, RenderableType::Movable
			, vec2f(50.0f, 50.0f)
			, Material{ GetTexture(&Resources, "star"), vec4f{ 1, 1, 1, 1 } });
	
		Game_Scene.RendererManager.AddComponent("platform"
			, RenderableType::Static
			, vec2f(screenWidth * 2.0f, 50.0f)
			, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });
	}

	{
		//Collision component

		// Addcomponent (actorName , vec3f *pos , vec2 size ,bool ground ,bool wall ,bool trigger)

		Game_Scene.CollisionManager.AddComponent("KAI2", &Game_Scene.ActorManager.GetTransform("KAI2")->Position,
			Game_Scene.RendererManager.GetRenderable("KAI2", RenderableType::Movable)->Size, false, false, false);

		Game_Scene.CollisionManager.AddComponent("brick", &Game_Scene.ActorManager.GetTransform("brick")->Position,
			Game_Scene.RendererManager.GetRenderable("brick", RenderableType::Movable)->Size
            , true, false, false);

	/*	
		Game_Scene.CollisionManager.AddComponent("wall", &Game_Scene.ActorManager.GetTransform("wall")->Position,
			Game_Scene.RendererManager.GetRenderable("wall")->Size
			, true, true, false);
			*/

		Game_Scene.CollisionManager.AddComponent("player", &Game_Scene.ActorManager.GetTransform("player")->Position,
			vec2f(50.0f, 175.0f), false, false, false);

		Game_Scene.CollisionManager.AddComponent("platform", &Game_Scene.ActorManager.GetTransform("platform")->Position,
			Game_Scene.RendererManager.GetRenderable("platform", RenderableType::Static)->Size
			, true, false, false);
	}

//	Game_Scene.MainCamera.Eye = Game_Scene.ActorManager.GetTransform("player")->Position;
}


void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

