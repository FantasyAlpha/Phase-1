#include<EnemyWarriorMind.h>

EnemyWarriorMind::EnemyWarriorMind(char* comName, Game_Resources *resources, char* name, vec3f pos, char* attackName, char* hbBarName, char* idleAName, char* walkAname, char* attackAName, char* colliderName, char* spriteName)

{
	Resources = resources;

	myName = name;
	this->Type = "Warrior Mind";
	this->Name = comName;
	this->Size = sizeof(EnemyWarriorMind);
	myAttackName = attackName;
	myHbBarName = hbBarName;
	myIdleAName = idleAName;
	myWalkAName = walkAname;
	myAttackAName = attackAName;
	myColliderName = colliderName;
	mySpriteName = spriteName;
	myPos= pos;

	//create warrior actor and it's component
	

	//create warrior HP Bar actor and it's component
	

	}

void EnemyWarriorMind::Init()
{
	player = this->Scene->GetGameObject("player");
	me = this->Scene->GetGameObject(myName);
	me->ObjectTransform.Position = myPos;

	HpBar = this->Scene->AddObject(myHbBarName);
	HpBar->ObjectTransform.Position = myPos;
	HpBar->ObjectTransform.Position.Y += 200;
	HpBar->AddComponent(&SpriteRenderer("HpBar_sprite", Sprite{ vec3f(0, 0, 0), vec2f(150, 25), Material{ GetTexture(Resources, "Empty"), vec4f(0, 1, .5, 1) } }));
}

 void EnemyWarriorMind::Update(float delta, Game_Input *input)
{
	//my HP follow me 
	HpBar->ObjectTransform.Position.X = me->ObjectTransform.Position.X;
	HpBar->ObjectTransform.Position.Y = me->ObjectTransform.Position.Y + 250;
	//update my Hp
	HpBar->ObjectTransform.Scale.X = (myHp / 100.0f)*1.0f;


	//timer for make the colider of attack wait a little then appear on left
	if (showColiderAfter){ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionL = true; showColiderAfter = false; showColiderAfterTimer = 0; }
	//timer for make the colider of attack wait a little then appear on right
	if (showColiderAfterR){ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionR = true; showColiderAfterR = false; showColiderAfterTimer = 0; }

	//timer for make the enemy wait a little after an attack

	if (isAttackWaiting){ waitAttackTimer++; }

	if (waitAttackTimer == waitAttackTime){ isAttackWaiting = false; waitAttackTimer = 0; }
	//destroy colider after timer count
	if (attackCollisionOn){
		attackCollisionOn = false;
		if (this->Scene->GetGameObject(myAttackName) != NULL){
			this->Scene->RemoveObject(myAttackName);
		}
	}
	// create and show colider on left
	if (fireAttackCollisionL)
	{
		Attack = this->Scene->AddObject(myAttackName);
		Attack->Layer = "warrior";
		Attack->ObjectTransform.Position = me->ObjectTransform.Position;
		Attack->AddComponent(&Collider("cp", &Attack->ObjectTransform.Position, vec2f(20, 100), vec2f(0, 0), false, true));
		Attack->ObjectTransform.Position.X -= 140;
		Collider* aCollider = (Collider*)Attack->GetComponent("cp","COLLIDER");
		aCollider->velocity.X = -140;


		/*Scene->ActorManager.AddActor(myAttackName, Transform{});
		Scene->ActorManager.GetTransform(myAttackName)->Position = Scene->ActorManager.GetTransform(myName)->Position;
		Scene->RendererManager.AddComponent(myAttackName, RenderableType::Movable, vec3f(), vec2f(50, 100), Material{ GetTexture(Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });
		Scene->CollisionManager.AddComponent(myAttackName, &Scene->ActorManager.GetTransform(myAttackName)->Position, vec2f(10, 100), false, false, true);
		Scene->ActorManager.GetTransform(myAttackName)->Position.X -= 140;
		Scene->CollisionManager.GetCollider(myAttackName)->velocity.X = -140;

		*/
		fireAttackCollisionL = false;
		attackCollisionOn = true;
	}

	// create and show colider on right
	if (fireAttackCollisionR)
	{
		Attack = this->Scene->AddObject(myAttackName);
		Attack->Layer = "warrior";
		Attack->ObjectTransform.Position = Owner->ObjectTransform.Position;
		Attack->AddComponent(&Collider("cp", &Attack->ObjectTransform.Position, vec2f(20, 100), vec2f(0, 0), false, true));
		Attack->ObjectTransform.Position.X += 140;
		// Attack->GetComponent("cp");
		Collider* aCollider = (Collider*)Attack->GetComponent("cp","COLLIDER");
		aCollider->velocity.X = 140;

		/*
		Scene->ActorManager.AddActor(myAttackName, Transform{});
		Scene->ActorManager.GetTransform(myAttackName)->Position = Scene->ActorManager.GetTransform(myName)->Position;

		Scene->RendererManager.AddComponent(myAttackName, RenderableType::Movable, vec3f(), vec2f(50, 100), Material{ GetTexture(Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });
		Scene->CollisionManager.AddComponent(myAttackName, &Scene->ActorManager.GetTransform(myAttackName)->Position, vec2f(10, 100), false, false, true);

		Scene->ActorManager.GetTransform(myAttackName)->Position.X += 140;
		Scene->CollisionManager.GetCollider(myAttackName)->velocity.X = 140;
		*/

		fireAttackCollisionR = false;
		attackCollisionOn = true;
	}
	Animator *animatorEnemy = (Animator *)Owner->GetComponent("animatorWarriorEnemy", "ANIMATOR");
	//if in wait state do nothing
	if (isAttackWaiting)
	{

	}
	//else play the logic
	else{
		//if the player in see range 

		if (std::abs(player->ObjectTransform.Position.X - me->ObjectTransform.Position.X) < seeRange)
		{
			//if the player in Attack range 
			if (std::abs(player->ObjectTransform.Position.X - me->ObjectTransform.Position.X) < attackRange)
			{
				//player in attack range from left
				if (player->ObjectTransform.Position.X < me->ObjectTransform.Position.X)
				{
					me->ObjectTransform.Scale.X = -1;

					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "wAttack"));
					animatorEnemy->SwitchClip("warrior basicattack clip");

					showColiderAfter = true;
					isAttackWaiting = true;



				}
				//player in attack range from right
				else
				{
					me->ObjectTransform.Scale.X = 1;

					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "wAttack"));
					animatorEnemy->SwitchClip("warrior basicattack clip");
					showColiderAfterR = true;
					isAttackWaiting = true;

				}
			}
			//Player in see range from left
			else if (player->ObjectTransform.Position.X < me->ObjectTransform.Position.X)
			{
				me->ObjectTransform.Scale.X = -1;
				me->ObjectTransform.Position.X -= 3;
				k = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
				k->velocity.X = -3;


				//Scene->CollisionManager.GetCollider(myName)->velocity.X = -3;



				//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources, "warrior walk"));
				animatorEnemy->SwitchClip("warrior walk clip");
			}
			//Player in see range from right
			else
			{
				me->ObjectTransform.Scale.X = 1;
				me->ObjectTransform.Position.X += 3;
				// Attack->GetComponent("cp");
				k = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
				k->velocity.X = 3;


				//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources, "warrior walk"));
				animatorEnemy->SwitchClip("warrior walk clip");
			}

		}
		//Player not in see range
		else
		{

			animatorEnemy->SwitchClip("warrior idle clip");
			//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myIdleAName, &GetTexture(Resources, "warrior idle"));
		}

	}

	

	//colide part

	//Timer to make the enemy invulnerable after get hit by player for a short timr
	if (holdColide){ holdColideTimer++; }
	if (holdColideTimer == 180)
	{
		holdColide = false;
		holdColideTimer = 0;
		//EditSprite(&Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Buffers, vec3f(), Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Size, (.5, 1, 1, 1)); 
		//Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->RenderableMaterial.MeshColor = vec4f(1, 1, 1, 1);
		s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

		EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));

	}


	if (holdColide == false){
		Collider *ownerCollider = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
		for (int i = 0; i < ownerCollider->checkObjectCollision().size(); i++){
			//std::cout << "arr" << ownerCollider->checkObjectCollision()[i] << std::endl;
			//std::cout << "collided names    " << sceneManager->CollisionManager.checkActorCollision(myName)[i] << std::endl;
			if ((ownerCollider->checkObjectCollision())[i] == "playerBasicAttack"){
				// do......
				Owner->ObjectTransform.Position.X = Owner->ObjectTransform.Position.X
					- 50 * Owner->ObjectTransform.Scale.X;

				myHp -= 15;
				holdColide = true;
				//EditSprite(&Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Buffers, vec3f(), Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Size, (1, .5, .5, 1));
				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));
			}
			
			else if (ownerCollider->checkObjectCollision()[i] == "straightMonster"){
				// do......
				/*Scene->ActorManager.GetTransform(myName)->Position.X = Scene->ActorManager.GetTransform(myName)->Position.X
				- 100 * Scene->ActorManager.GetTransform(myName)->Scale.X;*/

				myHp -= 15;
				holdColide = true;

				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");
				
				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));
			}
			else if (ownerCollider->checkObjectCollision()[i] == "ArcMonster"){
				// do......
				myHp -= 15;
				holdColide = true;
				/*s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "COLLIDER");
				s->SpriteData.Skin.MeshColor = vec4f(1, 1, 1, .3);*/
				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));

			}
		}
	}
	//if Hp reach 0 Destroy me
	if (myHp <= 0)
	{ 
		this->Scene->RemoveObject(myHbBarName);
		this->Scene->RemoveObject(myName);
		
	}
}