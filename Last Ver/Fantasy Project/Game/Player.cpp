#include "Player.h"

void Player::Move(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input, bool Run)
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
			sceneManager->ActorManager.GetTransform(ActorName)->Scale.Y = 1.2;
			sceneManager->AnimationManager.SwitchAnimation(ActorName, RenderableType::Movable, "noe walk clip", &GetTexture(gameResource, "noe walk"));
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
			sceneManager->ActorManager.GetTransform(ActorName)->Scale.Y = 1.2;
			sceneManager->AnimationManager.SwitchAnimation(ActorName, RenderableType::Movable, "noe walk clip", &GetTexture(gameResource, "noe walk"));
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
void Player::Colide()
{
	for (int i = 0; i < sceneManager->CollisionManager.checkActorCollision(myName).size(); i++){
		//std::cout << "collided names    " << sceneManager->CollisionManager.checkActorCollision(myName)[i] << std::endl;
		if (sceneManager->CollisionManager.checkActorCollision(myName)[i] == "enemy1 Attack"){
			// do......
			sceneManager->ActorManager.GetTransform(myName)->Position.X = sceneManager->ActorManager.GetTransform(myName)->Position.X
				- 100 * sceneManager->ActorManager.GetTransform(myName)->Scale.X;
			myHp -= 5;
		}
	}
}

void Player::jumpHandle(char*ActorName, Game_Input *input, float delta)
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
void Player::StraigthMonster(char *ActorName, float delta, Game_Input *input)
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
	if (input->S.KeyDown && waitStraight_falg == false)
	{
		sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe summon clip", &GetTexture(gameResource, "noe summon"));
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

		sceneManager->ActorManager.GetTransform(ActorName)->Position.X = sceneManager->ActorManager.GetTransform("player")->Position.X + 50 * sceneManager->ActorManager.GetTransform("player")->Scale.X;
		sceneManager->ActorManager.GetTransform(ActorName)->Position.Y = sceneManager->ActorManager.GetTransform("player")->Position.Y;
		sceneManager->ActorManager.GetTransform(ActorName)->Scale.X = sceneManager->ActorManager.GetTransform("player")->Scale.X;

		myHp -= 20;
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


void Player::ArcMonster(char *ActorName, float delta, Game_Input *input)
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
		sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe summon clip", &GetTexture(gameResource, "noe summon"));
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
		sceneManager->ActorManager.GetTransform(ActorName)->Position.X = sceneManager->ActorManager.GetTransform("player")->Position.X + 50 * sceneManager->ActorManager.GetTransform("player")->Scale.X;
		sceneManager->ActorManager.GetTransform(ActorName)->Position.Y = sceneManager->ActorManager.GetTransform("player")->Position.Y;
		sceneManager->ActorManager.GetTransform(ActorName)->Scale.X = sceneManager->ActorManager.GetTransform("player")->Scale.X;
		x_ArcVelo = sin(arcMonDirection*PI / 180)*arcMonStrenght;
		y_ArcVelo = cos(arcMonDirection*PI / 180)*arcMonStrenght;
		diro = sceneManager->ActorManager.GetTransform("player")->Scale.X;
		//Game_Scene.RendererManager.GetRenderable(ActorName)->Skip = false;
		myHp -= 10;
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


void Player::PullBack(char *ActorName, vec2f goalVelocity, float delta, Game_Input *input)
{

	if (waitPull_falg == true && waitPull > 0)
		waitPull--;
	if (waitPull == 0)
	{
		waitPull_falg = false;
		waitPull = 121;

	}
	if (input->W.KeyDown && waitPull_falg == false)
	{
		sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe summon clip", &GetTexture(gameResource, "noe summon"));
		
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

void Player::BasicAttack(Game_Input *input)
{
	//timer for make the colider of attack wait a little then appear on left
	if (showColiderAfter){ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionL = true; showColiderAfter = false; showColiderAfterTimer = 0; }
	//timer for make the colider of attack wait a little then appear on right
	if (showColiderAfterR){ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionR = true; showColiderAfterR = false; showColiderAfterTimer = 0; }

	//timer for make the enemy wait a little after an attack

	if (isAttackWaiting){ waitAttackTimer++; }

	if (waitAttackTimer == waitAttackTime){ isAttackWaiting = false; waitAttackTimer = 0; }

	if (attackCollisionOn){ attackCollisionOn = false; sceneManager->ActorManager.DestroyActor("playerBasicAttack"); }
	if (fireAttackCollisionL)
	{
		sceneManager->ActorManager.AddActor("playerBasicAttack", Transform{});
		sceneManager->ActorManager.GetTransform("playerBasicAttack")->Position = sceneManager->ActorManager.GetTransform(myName)->Position;

		sceneManager->RendererManager.AddComponent("playerBasicAttack", RenderableType::Movable, vec3f(),
			vec2f(50, 100), Material{ GetTexture(gameResource, "Empty"), vec4f{ 1, 1, 1, 0 } });
		sceneManager->CollisionManager.AddComponent("playerBasicAttack", 
			&sceneManager->ActorManager.GetTransform("playerBasicAttack")->Position,vec2f(50,100), false, false, true);

		sceneManager->ActorManager.GetTransform("playerBasicAttack")->Position.X -= 140;
		sceneManager->CollisionManager.GetCollider("playerBasicAttack")->velocity.X = -140;


		fireAttackCollisionL = false;
		attackCollisionOn = true;
	}


	if (fireAttackCollisionR)
	{
		sceneManager->ActorManager.AddActor("playerBasicAttack", Transform{});
		sceneManager->ActorManager.GetTransform("playerBasicAttack")->Position = sceneManager->ActorManager.GetTransform(myName)->Position;

		sceneManager->RendererManager.AddComponent("playerBasicAttack", RenderableType::Movable, vec3f(), vec2f(100, 100), Material{ GetTexture(gameResource, "Empty"), vec4f{ 1, 1, 1, 0 } });
		sceneManager->CollisionManager.AddComponent("playerBasicAttack", &sceneManager->ActorManager.GetTransform("playerBasicAttack")->Position, vec2f(100, 200), false, false, true);



		sceneManager->ActorManager.GetTransform("playerBasicAttack")->Position.X += 140;
		sceneManager->CollisionManager.GetCollider("playerBasicAttack")->velocity.X = 140;


		fireAttackCollisionR = false;
		attackCollisionOn = true;
	}


	if (isAttackWaiting)
	{

	}
	else
	{
		if (input->D.KeyDown && sceneManager->ActorManager.GetTransform(myName)->Scale.X<0)
		{
			sceneManager->ActorManager.GetTransform(myName)->Scale.X = -10;
			sceneManager->ActorManager.GetTransform(myName)->Scale.Y = 1.4;
			sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe attack clip", &GetTexture(gameResource, "noe basicattack"));
			sceneManager->ActorManager.GetTransform(myName)->Scale.X = -1.8;
			sceneManager->ActorManager.GetTransform(myName)->Scale.Y = 1.8;
			sceneManager->ActorManager.GetTransform(myName)->Position.Y = sceneManager->ActorManager.GetTransform(myName)->Position.Y+120;
			showColiderAfter = true;
			isAttackWaiting = true;



		}
		else if (input->D.KeyDown && sceneManager->ActorManager.GetTransform(myName)->Scale.X>0)
		{
			sceneManager->ActorManager.GetTransform(myName)->Scale.X = 10;
			sceneManager->ActorManager.GetTransform(myName)->Scale.Y = 1.4;
			sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe attack clip", &GetTexture(gameResource, "noe basicattack"));
			sceneManager->ActorManager.GetTransform(myName)->Scale.X = 1.2;
			sceneManager->ActorManager.GetTransform(myName)->Scale.Y = 1.7;
			showColiderAfterR = true;
			isAttackWaiting = true;

		}
	}
}

Player::Player(SceneManager* scene, Game_Resources* resource, char* name, vec3f pos, vec2f size, char* textureName)
{
	sceneManager = scene;
	gameResource = resource;
	myName = name;
	sceneManager->ActorManager.AddActor(name, Transform{});
	sceneManager->ActorManager.GetTransform(name)->Position = pos;

	sceneManager->RendererManager.AddComponent(name,
		RenderableType::Movable,
		vec3f(),
		size,
		Material{ GetTexture(gameResource, textureName), vec4f(1, 1, 1, 1) },sceneManager->AnimationManager.GetAnimationClip("noe idle clip"));

	sceneManager->CollisionManager.AddComponent(name, &sceneManager->ActorManager.GetTransform(name)->Position,
		vec2f(100.0f, 250.0f), false, false, false);


	sceneManager->ActorManager.AddActor("hpBarPlayer", Transform{});
	sceneManager->ActorManager.GetTransform("hpBarPlayer")->Position = vec3f(-500,190,0);

	sceneManager->RendererManager.AddComponent("hpBarPlayer", RenderableType::Movable, vec3f(), vec2f(150, 25), Material{ GetTexture(gameResource, "Empty"), vec4f{ 0, 1, .5, 1 } });
}

void  Player::Update(Game_Input* input)
{
	//my HP follow Camera
	sceneManager->ActorManager.GetTransform("hpBarPlayer")->Position.X = sceneManager->MainCamera.Eye.X-500;
	//update my Hp
	sceneManager->ActorManager.GetTransform("hpBarPlayer")->Scale.X = (myHp / 100.0f)*1.0f;

	Move("player", 400.0f, .016, input, false);

	sceneManager->CollisionManager.CheckGroundCollision("player");

	jumpHandle("player", input, 0.016f);

	BasicAttack(input);
	StraigthMonster("straight_Mon", .016, input);
	ArcMonster("arc_Mon", .016, input);
	PullBack("player", 400.0f, 0.016, input);

	if (myHp <= 0)
	{
		EditSprite(&sceneManager->RendererManager.GetRenderable(myName, RenderableType::Movable)->Buffers, vec3f(), sceneManager->RendererManager.GetRenderable(myName, RenderableType::Movable)->Size, (1, 1, 1, 0));
	}
	 if (!input->LEFT.KeyDown && !input->RIGHT.KeyDown && !input->S.KeyDown && !input->A.KeyDown && !input->W.KeyDown && !input->D.KeyDown)
	{
		sceneManager->ActorManager.GetTransform(myName)->Scale.Y = 1;
		if (sceneManager->ActorManager.GetTransform(myName)->Scale.X>0){ sceneManager->ActorManager.GetTransform(myName)->Scale.X = 1; }
		
		else{ sceneManager->ActorManager.GetTransform(myName)->Scale.X = -1; }
			
		sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe idle clip", &GetTexture(gameResource, "noe idle"));
	}


}

