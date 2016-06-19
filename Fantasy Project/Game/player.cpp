#include "player.h"

void player::Move(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input, bool Run)
{

	currentvelocity.X = sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X;
	
		if ((input->RIGHT.KeyUp) && (input->LEFT.KeyUp)/*!input->LEFT.KeyDown&&!input->RIGHT.KeyDown*/){
			sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = 0.0f;
	
			currentvelocity.X = 0.0f;
		}
		else if (input->RIGHT.KeyDown)
		{
			if (sceneManager->CollisionManager.GetCollider(ActorName)->rigth == false)
			{
				//currentvelocity.X = Accelerate(goalVelocity.X, currentvelocity.X, delta);
				currentvelocity.X = goalVelocity.X*delta;
	
				sceneManager->ActorManager.GetTransform(ActorName)->Position.X += currentvelocity.X;
				sceneManager->ActorManager.GetTransform(ActorName)->Scale.X = 1;
				sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = currentvelocity.X;
			}
		}
		else if (input->LEFT.KeyDown)
		{
			if (sceneManager->CollisionManager.GetCollider(ActorName)->left == false)
			{
				//currentvelocity.X = -1.0f * Accelerate(goalVelocity.X, currentvelocity.X, delta);
	
				currentvelocity.X = -1.0f*goalVelocity.X*delta;
	
				sceneManager->ActorManager.GetTransform(ActorName)->Position.X += currentvelocity.X;
	
				sceneManager->ActorManager.GetTransform(ActorName)->Scale.X = -1;
	
				sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = currentvelocity.X;
			}
		}	
	
		AutoMoveCameraHorizontal(&sceneManager->MainCamera, currentvelocity, sceneManager->ActorManager.GetTransform(ActorName)->Position,
			sceneManager->CollisionManager.GetCollider(ActorName)->size);
	
		//std::cout << "player velocity " << currentvelocity.X << std::endl;
}

void player :: jumpHandle(char*ActorName, Game_Input *input, float delta)
{
	bool groundCheck = false;

	bool Jump;

	controlWait++;

	Jump = sceneManager->CollisionManager.GetCollider(ActorName)->jump;

	if (waitBeforeDouble_flag == true && waitDouble > 0)
		waitDouble--;

	if (waitDouble == 0)
	{
		waitBeforeDouble_flag = false;
		waitDouble = 121;

	}
	if (Jump)
	{

		groundCheck = true;
	}

	if (groundCheck)

	{
		doubleJumpFlag = false;
		onair = false;
		fallSpeed_jump = 0;
		sceneManager->CollisionManager.GetCollider(ActorName)->velocity.Y = 0;

	}

	if (input->Space.KeyDown && groundCheck)
	{
		// reset 
		jumpSpeed = 17.0f;
		onair = true;
		jumpflag = true;
		groundCheck = false;
		controlWait = 0;
	}

	else if (input->Space.KeyDown && onair && waitBeforeDouble_flag == false)
	{
		if (controlWait >= 17)
		{
			doubleJumpFlag = true;
			onair = false;
			jumpflag = true;
			waitBeforeDouble_flag = true;
			controlWait = 0;
		}
	}

	if (sceneManager->CollisionManager.GetCollider(ActorName)->up)
	{
		jumpflag = false;
	}

	if (jumpflag)
	{
		fallSpeed_jump = 0;
		
		sceneManager->ActorManager.GetTransform(ActorName)->Position.Y += jumpSpeed;
		//	Game_Scene.CollisionManager.GetCollider(ActorName)->pos->Y += jumpSpeed;
		sceneManager->CollisionManager.GetCollider(ActorName)->velocity.Y = jumpSpeed;

		jumpSpeed -= sceneManager->CollisionManager.physics.Gravity*delta * 10.0f;

		if (jumpSpeed <= 0){
			if (doubleJumpFlag == true && onair == false)
			{
				jumpSpeed = 0;

				jumpSpeed += 13.0f;
				doubleJumpFlag = false;

			}
			else {
				jumpSpeed = 0;
				jumpflag = false;
			}
		}
	}
	else if (!groundCheck)
	{

		fallSpeed_jump += (sceneManager->CollisionManager.physics.Gravity*delta*15.0f);

		sceneManager->ActorManager.GetTransform(ActorName)->Position.Y -= fallSpeed_jump;
		sceneManager->CollisionManager.GetCollider(ActorName)->velocity.Y = fallSpeed_jump;

	}

}
void player :: StraigthMonster(char *ActorName, float delta, Game_Input *input)
{
	if (straight_hideFlag == true)
	{
		sceneManager->ActorManager.DestroyActor("straight_Mon");
		//Game_Scene.RendererManager.GetRenderable(ActorName)->Skip = true;
	}

	if (waitStraight_falg == true && waitStraight > 0)
		waitStraight--;
	if (waitStraight == 0)
	{
		waitStraight_falg = false;
		waitStraight = 121;

	}
	if (input->UP.KeyDown && waitStraight_falg == false)
	{
		straight_hideFlag = false;
		sceneManager->ActorManager.AddActor("straight_Mon", Transform{});

		//	Game_Scene.ActorManager.GetTransform("straight_Mon")->Position = vec3f(200.0f, -200.0f, 0);

		sceneManager->RendererManager.AddComponent("straight_Mon"
			, RenderableType::Movable
			, vec3f()
			, vec2f(100, 200)
			, Material{ GetTexture(gameResource, "straight_Mon"), vec4f{ 1, 1, 1, 1 } });


		sceneManager->CollisionManager.AddComponent("straight_Mon", &sceneManager->ActorManager.GetTransform(ActorName)->Position,
			vec2f(50.0f, 175.0f), false, false, true);

		//	Game_Scene.RendererManager.GetRenderable(ActorName)->Skip = false;


		straight_flag = true;
		waitStraight_falg = true;
		sceneManager->ActorManager.GetTransform(ActorName)->Position.X = sceneManager->ActorManager.GetTransform("player")->Position.X 
			+ 50 * sceneManager->ActorManager.GetTransform("player")->Scale.X;
		sceneManager->ActorManager.GetTransform(ActorName)->Position.Y = sceneManager->ActorManager.GetTransform("player")->Position.Y;
		sceneManager->ActorManager.GetTransform(ActorName)->Scale.X = sceneManager->ActorManager.GetTransform("player")->Scale.X;
	}

	if (straight_flag == true && straightCount <40)
	{
		sceneManager->ActorManager.GetTransform(ActorName)->Position.X += 10.0f*sceneManager->ActorManager.GetTransform(ActorName)->Scale.X;
		sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = 10.0f*sceneManager->ActorManager.GetTransform(ActorName)->Scale.X;

		straightCount++;
		if (straightCount > 38)
		{
			straight_flag = false;
			straightCount = 0;
			straight_hideFlag = true;

			//Game_Scene.ActorManager.DestroyActor("straight_Mon");
			//Game_Scene.RendererManager.RemoveRenderer("straight_Mon");
			//Game_Scene.CollisionManager.RemoveComponent("straight_Mon");
		}
	}


}


void player :: ArcMonster(char *ActorName, float delta, Game_Input *input)
{
	if (arcMon_hideFlag == true)
	{
		sceneManager->ActorManager.DestroyActor(ActorName);
	}
	if (waitArcMon_falg == true && waitArcMon > 0)
		waitArcMon--;
	if (waitArcMon == 0)
	{
		waitArcMon_falg = false;
		waitArcMon = 121;

	}
	if (input->A.KeyDown && waitArcMon_falg == false)
	{
		arcMon_hideFlag = false;
		//Game_Scene.RendererManager.GetRenderable(ActorName)->Skip = false;
		sceneManager->ActorManager.AddActor(ActorName, Transform{});

		sceneManager->RendererManager.AddComponent(ActorName
			, RenderableType::Movable
			, vec3f()
			, vec2f(100.0f, 150.0f)
			, Material{ GetTexture(gameResource, "arc_Mon"), vec4f{ 1, 1, 1, 1 } });


		sceneManager->CollisionManager.AddComponent(ActorName, &sceneManager->RendererManager.GetRenderable(ActorName, RenderableType::Movable)->Position,
			vec2f(50.0f, 175.0f), false, false, true);



		arcMon_flag = true;
		waitArcMon_falg = true;
		sceneManager->ActorManager.GetTransform(ActorName)->Position.X = sceneManager->ActorManager.GetTransform("player")->Position.X 
			+ 50 * sceneManager->ActorManager.GetTransform("player")->Scale.X;
		sceneManager->ActorManager.GetTransform(ActorName)->Position.Y = sceneManager->ActorManager.GetTransform("player")->Position.Y;

		//sceneManager->ActorManager.GetTransform(ActorName)->Scale.X = sceneManager->ActorManager.GetTransform("player")->Scale.X;
		x_ArcVelo = sin(arcMonDirection*PI / 180)*arcMonStrenght;
		y_ArcVelo = cos(arcMonDirection*PI / 180)*arcMonStrenght;
		diro = sceneManager->ActorManager.GetTransform("player")->Scale.X;
		//Game_Scene.RendererManager.GetRenderable(ActorName)->Skip = false;
	}
	if (arcMon_flag == true && arcMonCount <65)
	{

	sceneManager->ActorManager.GetTransform(ActorName)->Position.X += x_ArcVelo*diro;
	sceneManager->ActorManager.GetTransform(ActorName)->Position.Y += y_ArcVelo;
	sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = x_ArcVelo*diro;
	sceneManager->CollisionManager.GetCollider(ActorName)->velocity.Y = y_ArcVelo;
		y_ArcVelo--;
		arcMonCount++;
		if (arcMonCount > 63)
		{
			arcMon_flag = false;
			arcMonCount = 0;
			arcMon_hideFlag = true;
		}
	}

}


void player :: PullBack(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input)
{

	if (waitPull_falg == true && waitPull > 0)
		waitPull--;
	if (waitPull == 0)
	{
		waitPull_falg = false;
		waitPull = 121;

	}
	if (input->S.KeyDown && waitPull_falg == false)
	{

		pullBack_flag = true;
		waitPull_falg = true;

	}
	if (pullBack_flag == true && pullCount<9)
	{

		sceneManager->ActorManager.GetTransform(ActorName)->Position.X -= (40.0f* sceneManager->ActorManager.GetTransform(ActorName)->Scale.X);
		sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = -40.0f* sceneManager->ActorManager.GetTransform(ActorName)->Scale.X;

		pullCount++;
		if (pullCount >7)
		{
			pullBack_flag = false;
			pullCount = 0;
		}
	}

}

player::player(SceneManager* scene, Game_Resources* resource, char* name, vec3f pos, vec2f size, char* textureName)
{
	sceneManager = scene;
	gameResource = resource;

	sceneManager->ActorManager.AddActor(name, Transform{});
	sceneManager->ActorManager.GetTransform(name)->Position = pos;
	
	sceneManager->RendererManager.AddComponent(name,
		RenderableType::Movable, 
		vec3f(),
		size, 
		Material{ GetTexture(gameResource, textureName), vec4f(1, 1, 1, 1) });

	sceneManager->CollisionManager.AddComponent(name, &sceneManager->ActorManager.GetTransform(name)->Position,
		vec2f(50.0f, 175.0f), false, false, false);
}

void  player::Update(Game_Input* input)
{
	Move("player", 400.0f, .016, input, false);

	sceneManager->CollisionManager.CheckGroundCollision("player");

	jumpHandle("player", input, 0.016f);

	StraigthMonster("straight_Mon", .016, input);
	ArcMonster("arc_Mon", .016, input);
	PullBack("player", 400.0f, 0.016, input);
	sceneManager->RendererManager.MainLight.Position = sceneManager->ActorManager.GetTransform("player")->Position + 100.0f;


}

