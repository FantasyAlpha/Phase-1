#include "Game.h"
#include <sstream> 
#include"player.h"
#include<vector>
global_variable Game_Resources Resources;

float screenWidth;
float screenHeight;
float UPS;

global_variable SceneManager Game_Scene;
player  Mainplayer;
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


//float fallSpeed_jump = 0.0f;
//float fallSpeed = 0.0f;
//bool jumpflag = false;
////float Gravity = 2.0f;
//float jumpSpeed = 13.0f;

//void ApplyGravity(char*ActorName, float delta){
//
//	bool groundCheck = false;
//	bool Jump;
//
//	Jump = Game_Scene.CollisionManager.GetCollider(ActorName)->jump;
//
//	if (Jump)
//	{
//
//		groundCheck = true;
//
//	}
//
//	if (groundCheck)
//	{
//		//Jump = true;
//		fallSpeed = 0;
//		Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.Y = 0;
//
//	}
//
//	else if (!groundCheck)
//	{
//
//		fallSpeed += (Game_Scene.CollisionManager.physics.Gravity*delta*25.0f);
//
//		Game_Scene.ActorManager.GetTransform(ActorName)->Position.Y -= fallSpeed;
//		//Game_Scene.CollisionManager.GetCollider(ActorName)->pos->Y -= fallSpeed;
//		Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.Y = fallSpeed;
//
//	}
//}


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
		Game_Scene.RendererManager.MainLight.Strength += 2;
	}
	
	
	if (input->DOWN.KeyDown)
	{
		Game_Scene.RendererManager.MainLight.Position.X -= 1;
	}

	
	

	Mainplayer.Update(input);

	Game_Scene.UpdateScene(0.016f);
	
	for (int i = 0; i < Game_Scene.CollisionManager.checkActorCollision("player").size(); i++){
		std::cout << "collided names    " << Game_Scene.CollisionManager.checkActorCollision("player")[i] << std::endl;
		if (Game_Scene.CollisionManager.checkActorCollision("player")[i] == "brick"){
			// do......
			Game_Scene.ActorManager.GetTransform("player")->Position.X = Game_Scene.ActorManager.GetTransform("player")->Position.X
				-100 * Game_Scene.ActorManager.GetTransform("player")->Scale.X;
		}
	}
	
}

GAME_DLL GAME_SHUTDOWN(Game_Shutdown)
{
}

void InitResources()
{
	/*
	*/
	//Initialize SDL AUDIO
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: %s\n", SDL_GetError();
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: \n";
	}

	// add resources : Textures
	//AddTexture(&Resources, LoadTexture("resources\\textures\\noe walk.png"), "noe walk");
	AddTexture(&Resources, LoadTexture("resources\\textures\\noe walk.png"), "noe idle");
	AddTexture(&Resources, LoadTexture("resources\\textures\\empty.png"), "Empty");
	//AddTexture(&Resources, LoadTexture("resources\\textures\\player_2.png"), "PLAYER");
	AddTexture(&Resources, LoadTexture("resources\\textures\\Neo In.png"), "neo");
	AddTexture(&Resources, LoadTexture("resources\\textures\\back.png"), "Back");
	AddTexture(&Resources, LoadTexture("resources\\textures\\BG0.png"), "BG0");
	AddTexture(&Resources, LoadTexture("resources\\textures\\PlatForm2.png"), "PlatForm2");
	AddTexture(&Resources, LoadTexture("resources\\textures\\Genta.png"), "straight_Mon");
	AddTexture(&Resources, LoadTexture("resources\\textures\\yellow.png"), "arc_Mon");

	

	//AddSound(&Resources,)
	AddBGM(&Resources, "resources\\audio\\testClip.wav", "benny");
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

		Game_Scene.ActorManager.AddActor("wall", Transform{});
		Game_Scene.ActorManager.AddActor("brick", Transform{});


		//magic for to put things repeatedly 
		for (int i = -1; i < 4; i++)
		{

			std::stringstream s;
			s.clear();
			s << "BG" << i;

			char* backgroundActorName = new char[s.str().length()];

			std::strcpy(backgroundActorName, s.str().c_str());

			std::stringstream ss;
			ss.clear();
			ss << "TG" << i;

			char* TGActorName = new char[ss.str().length()];

			std::strcpy(TGActorName, ss.str().c_str());

			std::stringstream sss;
			sss.clear();
			sss << "G" << i;

			char* GActorName = new char[sss.str().length()];

			std::strcpy(GActorName, sss.str().c_str());


			Game_Scene.ActorManager.AddActor(backgroundActorName, Transform{});
			Game_Scene.ActorManager.AddActor(TGActorName, Transform{});
			Game_Scene.ActorManager.AddActor(GActorName, Transform{});


			Game_Scene.ActorManager.GetTransform(TGActorName)->Position = vec3f((1100.0f * i), -310.0f, 0);
			//Game_Scene.ActorManager.GetTransform(GActorName)->Position = vec3f((1100.0f * i), -310.0f, 0);

			Game_Scene.RendererManager.AddComponent(backgroundActorName
				, RenderableType::Static
				, vec3f((1150.0f * i), 68.0f, 0.0f)
				, vec2f(screenWidth, 585)
				, Material{ GetTexture(&Resources, "BG0"), vec4f{ .5f, .5f, 1, 1 } });

			Game_Scene.RendererManager.AddComponent(GActorName
				, RenderableType::Static, vec3f((1100.0f * i), -310.0f, 0), vec2f(screenWidth, 240.0f)
				, Material{ GetTexture(&Resources, "PlatForm2"), vec4f{ 0, .3f, .8f, 1 } });


			Game_Scene.RendererManager.AddComponent(TGActorName
				, RenderableType::Static, vec3f((1100.0f * i), -310, 0), vec2f(screenWidth, 60)
				, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });

			Game_Scene.CollisionManager.AddComponent(TGActorName, &Game_Scene.RendererManager.GetRenderable(TGActorName, RenderableType::Static)->Position,
				Game_Scene.RendererManager.GetRenderable(TGActorName, RenderableType::Static)->Size
				, true, false, false);
		}

	}

	{
	// --------------backgrounds

	Game_Scene.ActorManager.AddActor("BG5", Transform{});
	Game_Scene.ActorManager.GetTransform("BG5")->Position = vec3f((1150.0f * 4), 68.0f, 0.0f);

	Game_Scene.RendererManager.AddComponent("BG5"
		, RenderableType::Static
		, vec3f((1150.0f * 4), 68.0f, 0.0f)
		, vec2f(screenWidth, 585)
		, Material{ GetTexture(&Resources, "BG0"), vec4f{ .5f, .5f, 1, 1 } });
	
	//-------------------------platforms and thier collison 

	Game_Scene.ActorManager.AddActor("DropPlatform", Transform{});
	Game_Scene.ActorManager.AddActor("DropPLAatformCollision", Transform{});

	Game_Scene.ActorManager.GetTransform("DropPlatform")->Position = vec3f((1100.0f * 4)+200, -310.0f, 0);
	Game_Scene.ActorManager.GetTransform("DropPLAatformCollision")->Position = vec3f((1100.0f * 4) + 200, - 310.0f, 0);

	Game_Scene.RendererManager.AddComponent("DropPlatform"
		, RenderableType::Static, vec3f((1100.0f * 4) + 200, -310.0f, 0), vec2f(screenWidth, 240.0f)
		, Material{ GetTexture(&Resources, "PlatForm2"), vec4f{ 0, .3f, .8f, 1 } });

	Game_Scene.RendererManager.AddComponent("DropPLAatformCollision"
		, RenderableType::Static, vec3f((1100.0f * 4) + 200, -310, 0), vec2f(screenWidth, 60)
		, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });

	Game_Scene.CollisionManager.AddComponent("DropPLAatformCollision", &Game_Scene.RendererManager.GetRenderable("DropPLAatformCollision", RenderableType::Static)->Position,
		Game_Scene.RendererManager.GetRenderable("DropPLAatformCollision", RenderableType::Static)->Size
		, true, false, false);

	}

	{
		// Render components
		//Game_Scene.AnimationManager.GetAnimationClip("a1")
		Game_Scene.RendererManager.InitMainShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
		Game_Scene.RendererManager.InitDebugShader("resources\\shaders\\vertex shader 120_2.vert", "resources\\shaders\\fragment shader 120_2.frag");

	}
		{

			Game_Scene.ActorManager.GetTransform("brick")->Position = vec3f(100,-200,0);
			Game_Scene.RendererManager.AddComponent("brick"
				, RenderableType::Movable
				, vec3f()
				, vec2f(100, 100)
				, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, 1, 1, 1 } });

			Game_Scene.CollisionManager.AddComponent("brick", &Game_Scene.ActorManager.GetTransform("brick")->Position,
				Game_Scene.RendererManager.GetRenderable("brick", RenderableType::Movable)->Size
				, false, false, true);
			//Collision component

			// Addcomponent (actorName , vec3f *pos , vec2 size ,bool ground ,bool wall ,bool trigger)

			Mainplayer = player(&Game_Scene, &Resources, "player", vec3f(200.0f, -200.0f, 0), vec2f(200, 266), "neo");

		}

		//	Game_Scene.MainCamera.Eye = Game_Scene.ActorManager.GetTransform("player")->Position;
}