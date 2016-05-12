#include "Game.h"

global_variable Game_Resources Resources;

float screenWidth;
float screenHeight;

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
float Accelerate(float flGoal, float flCurrent, float delta){


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
			Game_Scene.MainCamera.Eye.X += currentvelocity.X;
			/*float temp = Game_Scene.TransformManager.GetTransform(ActorName)->Position.X;
			Game_Scene.CollisionManager.GetCollider(ActorName)->pos->X = temp;*/
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
			Game_Scene.MainCamera.Eye.X += currentvelocity.X;
			Game_Scene.ActorManager.GetTransform(ActorName)->Scale.X = -1;

			/*float temp = Game_Scene.TransformManager.GetTransform(ActorName)->Position.X;
			Game_Scene.CollisionManager.GetCollider(ActorName)->pos->X = temp;*/

			Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.X = currentvelocity.X;
		}
	}

	//std::cout << "player velocity " << currentvelocity.X << std::endl;
}

float fallSpeed = 0.0f;
bool jumpflag = false;
//float Gravity = 2.0f;
float jumpSpeed = 13.0f;
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
		jumpSpeed = 13.0f;

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
			jumpSpeed = 13.0f;

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
		Play_Music(GetBGM(&Resources, "benny"), 0);
	}
	if (input->D .KeyDown)
	{
		debug = false;
	}
	
	Game_Scene.ActorManager.GetTransform("KAI2")->Rotation.Z += 1;
	
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

	//Initialize SDL AUDIO
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: %s\n", SDL_GetError();
	}


	//Initialize SDL_mixer
	/* The first argument sets the sound frequency, and 44100 is a standard frequency that works on most systems.
	or you might use MIX_DEFAULT_FREQUENCY(22050) since that is a good value for most games.
	Most games use 22050, because 44100 requires too much CPU power on older computers.

	The second argument determines the sample format, which again here we're using the default.
	MIX_DEFAULT_FORMAT is the same as AUDIO_S16SYS which means (Signed 16-bit samples, in system byte order).

	The third argument is the number of hardware channels, and here we're using 2 channels for stereo.
	and 1 channel for mono.

	The last argument is the sample size, which determines the size of the chunks we use when playing sound.*/
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		// this means that open 44.1KHz, signed 16bit, system byte order,
		//      stereo audio, using 2048 byte chunks
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: \n";
		//printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	// add resources : Textures
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile1.png"), "Tile1");
	AddTexture(&Resources, LoadTexture("resources\\textures\\tile2.png"), "Tile2");
	AddTexture(&Resources, LoadTexture("resources\\textures\\empty.png"), "Empty");
	AddTexture(&Resources, LoadTexture("resources\\textures\\player_2.png"), "PLAYER");
	AddTexture(&Resources, LoadTexture("resources\\textures\\Neo In.png"), "neo");
	AddTexture(&Resources, LoadTexture("resources\\textures\\back.png"), "Back");
	AddTexture(&Resources, LoadTexture("resources\\textures\\star.png"), "star");
	AddTexture(&Resources, LoadTexture("resources\\textures\\grayCube.png"), "cube");


	AddBGM(&Resources, "resources\\audio\\testClip.wav", "benny");
}

void InitGameWorld()
{
	{
		Game_Scene.MainCamera = Camera(vec2f(screenWidth, screenHeight));
		Game_Scene.MainCamera.Type = CameraType::ORTHOGRAPHIC;
	}

	{
		Game_Scene.InitScene(100);
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

		Game_Scene.ActorManager.GetTransform("wall")->Position = vec3f(500.0f, 0.0f, 0);

		Game_Scene.ActorManager.GetTransform("player")->Position = vec3f(200.0f, -200.0f, 0);

	

	}

	{
		// Animation components

		uint32 frames[4];
		
		frames[0] = 4;
		frames[1] = 5;
		frames[2] = 6;
		frames[3] = 7;

		Game_Scene.AnimationManager.AddComponent("a1", 4, 2, frames, 4, 0.001f, true);
		
	}

	{
		// Render components
//Game_Scene.AnimationManager.GetAnimationClip("a1")
		Game_Scene.RendererManager.InitMainShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
		Game_Scene.RendererManager.InitDebugShader("resources\\shaders\\vertex shader 120_2.vert", "resources\\shaders\\fragment shader 120_2.frag");

		Game_Scene.RendererManager.AddComponent("backGround"
			, vec2f(screenWidth * 2.0f, screenHeight)
			, Material{ GetTexture(&Resources, "Back"), vec4f{ 1, 1, 1, 1 } });
		
		Game_Scene.RendererManager.AddComponent("brick"
			, vec2f(100.0f, 50.0f)
			, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, .3f, .5f, 1 } });

		Game_Scene.RendererManager.AddComponent("wall"
			, vec2f(50.0f, screenHeight)
			, Material{ GetTexture(&Resources, "Empty"), vec4f{ 0, .3f, .5f, 1 } });

		Game_Scene.RendererManager.AddComponent("player"
			, vec2f(100.0f, 200.0f)
			, Material{ GetTexture(&Resources, "neo"), vec4f{ 1, 1, 1, 1 } });
		Game_Scene.RendererManager.AddComponent("KAI2"
			, vec2f(50.0f, 50.0f)
			, Material{ GetTexture(&Resources, "star"), vec4f{ 1, 1, 1, 1 } });
	
		Game_Scene.RendererManager.AddComponent("platform"
			, vec2f(screenWidth * 2.0f, 50.0f)
			, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, 1, 1, 1 } });
	}

	{
		//Collision component

		// Addcomponent (actorName , vec3f *pos , vec2 size ,bool ground ,bool wall ,bool trigger)

		Game_Scene.CollisionManager.AddComponent("KAI2", &Game_Scene.ActorManager.GetTransform("KAI2")->Position,
			Game_Scene.RendererManager.GetRenderable("KAI2")->Size, false, false, false);

		Game_Scene.CollisionManager.AddComponent("brick", &Game_Scene.ActorManager.GetTransform("brick")->Position,
			Game_Scene.RendererManager.GetRenderable("brick")->Size
            , true, false, false);

		Game_Scene.CollisionManager.AddComponent("wall", &Game_Scene.ActorManager.GetTransform("wall")->Position,
			Game_Scene.RendererManager.GetRenderable("wall")->Size
			, true, true, false);

		Game_Scene.CollisionManager.AddComponent("player", &Game_Scene.ActorManager.GetTransform("player")->Position,
			vec2f(50.0f, 175.0f), false, false, false);

		Game_Scene.CollisionManager.AddComponent("platform", &Game_Scene.ActorManager.GetTransform("platform")->Position,
			Game_Scene.RendererManager.GetRenderable("platform")->Size
			, true, false, false);
	}


}


void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

