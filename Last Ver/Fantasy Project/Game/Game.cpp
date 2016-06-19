#include "Game.h"
#include <sstream> 
#include "EnemyWarrior.h"
#include "Player.h"

global_variable Game_Resources Resources;

float screenWidth;
float screenHeight;
float UPS;

global_variable SceneManager Game_Scene;
//Player Def
Player  Mainplayer;
//Enemy Def
EnemyWarrior e1;
EnemyWarrior e2;




//UI Variables
float lightXDif=0 , lightYDif=0;


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

	//std::cout << "player velocity " << currentvelocity.X << std::endl;
}

float fallSpeed_jump = 0.0f;
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
		fallSpeed_jump = 0;
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

	if (jumpflag&&fallSpeed_jump == 0){
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

		fallSpeed_jump += (Game_Scene.CollisionManager.physics.Gravity*delta*25.0f);

		Game_Scene.ActorManager.GetTransform(ActorName)->Position.Y -= fallSpeed_jump;
		//Game_Scene.CollisionManager.GetCollider(ActorName)->pos->Y -= fallSpeed_jump;
		Game_Scene.CollisionManager.GetCollider(ActorName)->velocity.Y = fallSpeed_jump;

	}
}

void ApplyGravity(char*ActorName, float delta){

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
	
	
	
	//UI & Light Follow Camera
	Game_Scene.RendererManager.MainLight.Position.X = Game_Scene.MainCamera.Eye.X - lightXDif;
	Game_Scene.RendererManager.MainLight.Position.Y = Game_Scene.MainCamera.Eye.Y - lightYDif;
	Game_Scene.ActorManager.GetTransform("main Canvas")->Position.X = Game_Scene.MainCamera.Eye.X - 420;
	
	

	
	//Check Ground
	Game_Scene.CollisionManager.CheckGroundCollision("player");
	if (Game_Scene.ActorManager.GetActor(e1.GetName()) != NULL){ Game_Scene.CollisionManager.CheckGroundCollision(e1.GetName()); }
	//if (Game_Scene.ActorManager.GetActor(e2.GetName()) != NULL){ Game_Scene.CollisionManager.CheckGroundCollision(e2.GetName()); }
	


	

	
	
	//Update Characters
	Mainplayer.Update(input);
	//enemy area
	if (Game_Scene.ActorManager.GetActor(e1.GetName()) != NULL){ ApplyGravity(e1.GetName(), delta);  e1.Update(); }
	//if (Game_Scene.ActorManager.GetActor(e2.GetName()) != NULL){ ApplyGravity(e2.GetName(), delta);  e2.Update(); }
	
	Game_Scene.UpdateScene(0.016f);

	//Colide Update Section
	Mainplayer.Colide();
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

	AddTexture(&Resources, LoadTexture("resources\\textures\\noe basicattack.png"), "noe basicattack");
	AddTexture(&Resources, LoadTexture("resources\\textures\\noe idle.png"), "noe idle");
	AddTexture(&Resources, LoadTexture("resources\\textures\\noe summon.png"), "noe summon");
	AddTexture(&Resources, LoadTexture("resources\\textures\\noe walk.png"), "noe walk");
	
	

	AddTexture(&Resources, LoadTexture("resources\\textures\\main Canvas.png"), "main Canvas");


	AddTexture(&Resources, LoadTexture("resources\\textures\\BG0.png"), "BG0");
	AddTexture(&Resources, LoadTexture("resources\\textures\\PlatForm2.png"), "PlatForm2");
	AddTexture(&Resources, LoadTexture("resources\\textures\\Genta.png"), "straight_Mon");
	AddTexture(&Resources, LoadTexture("resources\\textures\\weboo.png"), "arc_Mon");
	AddTexture(&Resources, LoadTexture("resources\\textures\\Warrior.png"), "Warrior");
	AddTexture(&Resources, LoadTexture("resources\\textures\\warrior idle.png"), "warrior idle");
	AddTexture(&Resources, LoadTexture("resources\\textures\\warrior walk.png"), "warrior walk");
	AddTexture(&Resources, LoadTexture("resources\\textures\\warrior attack3.png"), "wAttack");

	//AddSound(&Resources,)
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
	
	//Main Light

	{
		Game_Scene.RendererManager.MainLight.Strength = 400;
		Game_Scene.RendererManager.MainLight.Position.Y += 250;
		Game_Scene.RendererManager.MainLight.Position.X += 200;
	}

	//UI
	{
	lightXDif=	Game_Scene.MainCamera.Eye.X - Game_Scene.RendererManager.MainLight.Position.X;
	lightYDif = Game_Scene.MainCamera.Eye.Y - Game_Scene.RendererManager.MainLight.Position.Y;
	}


	{
		// Actor creations

				Game_Scene.ActorManager.AddActor("wall", Transform{});
	
		Game_Scene.ActorManager.AddActor("main Canvas", Transform{});
		

		//magic for to put things repeatedly (BackGround-Platform-Transparent Platform) 
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
				, RenderableType::Static,vec3f((1100.0f * i), -310, 0), vec2f(screenWidth, 60)
				, Material{ GetTexture(&Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });
			
			Game_Scene.CollisionManager.AddComponent(TGActorName, &Game_Scene.RendererManager.GetRenderable(TGActorName, RenderableType::Static)->Position,
				Game_Scene.RendererManager.GetRenderable(TGActorName, RenderableType::Static)->Size
				, true, false, false);
		}
	}

	{
		// Modify Transforms
		
		Game_Scene.ActorManager.GetTransform("main Canvas")->Position = vec3f(-420,240, 0);

	/*	Game_Scene.ActorManager.GetTransform("player")->Position = vec3f(200.0f, -200.0f, 0);*/

		

	}

	{
		// Animation components

		uint32 tenFrames[10];
		
		tenFrames[0] = 0;
		tenFrames[1] = 1;
		tenFrames[2] = 2;
		tenFrames[3] = 3;
		tenFrames[4] = 4;
		tenFrames[5] = 5;
		tenFrames[6] = 6;
		tenFrames[7] = 7;
		tenFrames[8] = 8;
		tenFrames[9] = 9;

		uint32 eightFrames[8];

		eightFrames[0] = 0;
		eightFrames[1] = 1;
		eightFrames[2] = 2;
		eightFrames[3] = 3;
		eightFrames[4] = 4;
		eightFrames[5] = 5;
		eightFrames[6] = 6;
		eightFrames[7] = 7;
		

		uint32 thirteenFrames[13];

		thirteenFrames[0] = 0;
		thirteenFrames[1] = 1;
		thirteenFrames[2] = 2;
		thirteenFrames[3] = 3;
		thirteenFrames[4] = 4;
		thirteenFrames[5] = 5;
		thirteenFrames[6] = 6;
		thirteenFrames[7] = 7;
		thirteenFrames[8] = 8;
		thirteenFrames[9] = 9;
		thirteenFrames[10] = 10;
		thirteenFrames[11] = 11;
		thirteenFrames[12] = 12;

		uint32 eighteenFrames[18];

		eighteenFrames[0] = 0;
		eighteenFrames[1] = 1;
		eighteenFrames[2] = 2;
		eighteenFrames[3] = 3;
		eighteenFrames[4] = 4;
		eighteenFrames[5] = 5;
		eighteenFrames[6] = 6;
		eighteenFrames[7] = 7;
		eighteenFrames[8] = 8;
		eighteenFrames[9] = 9;
		eighteenFrames[10] = 10;
		eighteenFrames[11] = 11;
		eighteenFrames[12] = 12;
		eighteenFrames[13] = 13;
		eighteenFrames[14] = 14;
		eighteenFrames[15] = 15;
		eighteenFrames[16] = 16;
		eighteenFrames[17] = 17;


		Game_Scene.AnimationManager.AddComponent("warrior walk clip", 1, 10, tenFrames, 10, 3.8f, true);

		Game_Scene.AnimationManager.AddComponent("warrior idle clip", 1, 8, eightFrames, 8, 7, true);
	
		Game_Scene.AnimationManager.AddComponent("warrior attack clip", 1, 10, tenFrames, 10, 2.0f, false);


		Game_Scene.AnimationManager.AddComponent("warrior2 walk clip", 1, 10, tenFrames, 10, 3.8f, true);

		Game_Scene.AnimationManager.AddComponent("warrior2 idle clip", 1, 8, eightFrames, 8, 7, true);

		Game_Scene.AnimationManager.AddComponent("warrior2 attack clip", 1, 10, tenFrames, 10, 2.0f, false);


		Game_Scene.AnimationManager.AddComponent("noe attack clip", 1, 11, thirteenFrames, 11, 2.0f, false);

		Game_Scene.AnimationManager.AddComponent("noe idle clip", 1, 18, eighteenFrames, 18, 5, true);

		Game_Scene.AnimationManager.AddComponent("noe summon clip", 1, 9, tenFrames, 9, 3.5, false);

		Game_Scene.AnimationManager.AddComponent("noe walk clip", 1, 10, tenFrames, 10, 3.8f, true);


		
	}

	{
		// Render components
//Game_Scene.AnimationManager.GetAnimationClip("a1")
		Game_Scene.RendererManager.InitMainShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
		Game_Scene.RendererManager.InitDebugShader("resources\\shaders\\vertex shader 120_2.vert", "resources\\shaders\\fragment shader 120_2.frag");

		/*
			*/
		
		Game_Scene.RendererManager.AddComponent("wall"
			, RenderableType::Static, vec3f(0,0,0)
			, vec2f(100, screenHeight)
			, Material{ GetTexture(&Resources, "Empty"), vec4f{ 0, 1, 1, 1 } });

		Game_Scene.RendererManager.AddComponent("main Canvas"
			, RenderableType::Movable, vec3f()
			, vec2f(320, 160)
			, Material{ GetTexture(&Resources, "main Canvas"), vec4f{ 1, 1, 1, 1 } });
		
		
		//Game_Scene.RendererManager.AddComponent("player"
		//	, RenderableType::Movable, vec3f()
		//	, vec2f(200, 266)
		//	, Material{ GetTexture(&Resources, "neo"), vec4f{ 1, 1, 1, 1 } });

	
	}

	{
		//Collision component

		// Addcomponent (actorName , vec3f *pos , vec2 size ,bool ground ,bool wall ,bool trigger)



		Game_Scene.CollisionManager.AddComponent("wall", &Game_Scene.RendererManager.GetRenderable("wall", RenderableType::Static)->Position,
			Game_Scene.RendererManager.GetRenderable("wall", RenderableType::Static)->Size
			, false, true, false );

		//Game_Scene.CollisionManager.AddComponent("player", &Game_Scene.ActorManager.GetTransform("player")->Position,
		//	vec2f(50.0f, 175.0f), false, false, false);	
	}

	//mainCharacter inilization
	{
		Mainplayer = Player(&Game_Scene, &Resources, "player", vec3f(200.0f, -200.0f, 0), vec2f(300, 266), "noe idle");
	}
	//enemy Area
		{
			e1 = EnemyWarrior(&Game_Scene, &Resources, "enemy1", vec3f(1000, -200.0f, 0.0f), vec2f(200.0f, 300.0f), "warrior idle", "enemy1 Attack","enemy1 hp","warrior idle clip","warrior walk clip","warrior attack clip");
			//e1 = EnemyWarrior(&Game_Scene, &Resources, "enemy2", vec3f(2000, -200.0f, 0.0f), vec2f(200.0f, 300.0f), "warrior idle", "enemy2 Attack", "enemy2 hp", "warrior2 idle clip", "warrior2 walk clip", "warrior2 attack clip");
			
		}


}


void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

