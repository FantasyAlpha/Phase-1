#include<EnemyRufusMind.h>

EnemyRufusMind::EnemyRufusMind(char* comName, Game_Resources *resources, char* name, vec3f pos, char* attackName, char* attackName2,char* hbBarName, char* idleAName, char* walkAname, char* attackAName, char* attack2AName, char* colliderName, char* spriteName)
{
	Resources = resources;

	myName = name;
	this->Type = "Rufus Mind";
	this->Name = comName;
	this->Size = sizeof(EnemyRufusMind);
	myAttackName = attackName;
	myAttackAdvanceName = attackName2;
	myHbBarName = hbBarName;
	myIdleAName = idleAName;
	myWalkAName = walkAname;
	myAttackAName = attackAName;
	myAttackAdvanceAname = attack2AName;
	myColliderName = colliderName;
	mySpriteName = spriteName;
	myPos = pos;

	//create warrior actor and it's component


	//create warrior HP Bar actor and it's component


}

void EnemyRufusMind::Init()
{
	player = this->Scene->GetGameObject("player");
	me = this->Scene->GetGameObject(myName);
	me->ObjectTransform.Position = myPos;


	HpBar = this->Scene->AddObject(myHbBarName);
	HpBar->AddComponent(&SpriteRenderer("HpBar_sprite", Sprite{ vec3f(0, 0, 0), vec2f(150, 25), Material{ GetTexture(Resources, "Empty"), vec4f(0, 1, .5, 1) } }));
	HpBar->ObjectTransform.Position = myPos;
	HpBar->ObjectTransform.Position.Y += 200;
	}


void EnemyRufusMind::Update(float delta, Game_Input *input)
{
	Animator *animatorEnemy = (Animator *)Owner->GetComponent("animatorRufusEnemy", "ANIMATOR");
	//my HP follow me 
	HpBar->ObjectTransform.Position.X = me->ObjectTransform.Position.X;
	HpBar->ObjectTransform.Position.Y = me->ObjectTransform.Position.Y + 250;
	//update my Hp
	HpBar->ObjectTransform.Scale.X = (myHp / 100.0f)*1.0f;







	//timer for make the colider of attack wait a little then appear on left (advance attack)
	if (showColiderAfterAdvance){ showColiderAfterAdvanceTimer++; }
	if (showColiderAfterAdvanceTimer == 35){ fireAttackCollisionAdvanceL = true; showColiderAfterAdvance = false; showColiderAfterAdvanceTimer = 0; }
	//timer for make the colider of attack wait a little then appear on right  (advance attack)
	if (showColiderAfterAdvanceR){ showColiderAfterAdvanceTimer++; }
	if (showColiderAfterAdvanceTimer == 35){ fireAttackCollisionAdvanceR = true; showColiderAfterAdvanceR = false; showColiderAfterAdvanceTimer = 0; }

	if (attackCollisionOnAdvance){
		attackCollisionOnAdvance = false;
		if (this->Scene->GetGameObject(myAttackAdvanceName) != NULL){
			this->Scene->RemoveObject(myAttackAdvanceName);
		}
	}

	if (fireAttackCollisionAdvanceL)
	{
		AttackAdvance = this->Scene->AddObject(myAttackAdvanceAname);
		AttackAdvance->Layer = "rufusAdvanceAttack";
		AttackAdvance->ObjectTransform.Position = me->ObjectTransform.Position;
		AttackAdvance->AddComponent(&Collider("cp", &AttackAdvance->ObjectTransform.Position, vec2f(500, 100), vec2f(0, 0), false, true));
		AttackAdvance->ObjectTransform.Position.X -= 500;
		Collider* aCollider = (Collider*)AttackAdvance->GetComponent("cp", "COLLIDER");
		aCollider->velocity.X = -500;

		/*Attack=this->Scene->AddObject(myAttackAdvanceName);
		Attack->ObjectTransform.Position = me->ObjectTransform.Position;

		Scene->RendererManager.AddComponent(myAttackAdvanceName, RenderableType::Movable, vec3f(), vec2f(300, 100), Material{ GetTexture(Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });
		Scene->CollisionManager.AddComponent(myAttackAdvanceName, &Scene->ActorManager.GetTransform(myAttackAdvanceName)->Position, vec2f(500, 100), false, false, true);
		*///std::cout << "poooos" << Scene->ActorManager.GetTransform(myName)->Position.Y << "    " << Scene->ActorManager.GetTransform(myAttackAdvanceName)->Position.Y << std::endl;

		/*Scene->ActorManager.GetTransform(myAttackAdvanceName)->Position.X -= 500;
		Scene->CollisionManager.GetCollider(myAttackAdvanceName)->velocity.X = -500;*/
		//make camera move toward player in y then try to make it with timer to change y pos random for certain times , it will be better if camera follow player directelly not with windowing
		//Scene->MainCamera.Eye.X += 100;

		fireAttackCollisionAdvanceL = false;
		attackCollisionOnAdvance = true;
	}

	// create and show colider on right
	if (fireAttackCollisionAdvanceR)
	{
		AttackAdvance = this->Scene->AddObject(myAttackAdvanceAname);
		AttackAdvance->Layer = "rufusAdvanceAttack";
		AttackAdvance->ObjectTransform.Position = me->ObjectTransform.Position;
		AttackAdvance->AddComponent(&Collider("cp", &AttackAdvance->ObjectTransform.Position, vec2f(500, 100), vec2f(0, 0), false, true));
		AttackAdvance->ObjectTransform.Position.X += 500;
		Collider* aCollider = (Collider*)AttackAdvance->GetComponent("cp", "COLLIDER");
		aCollider->velocity.X = 500;


		fireAttackCollisionAdvanceR = false;
		attackCollisionOnAdvance = true;
	}






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
		AttackAdvance->Layer = "rufusBasicAttack";
		Attack->ObjectTransform.Position = me->ObjectTransform.Position;
		Attack->AddComponent(&Collider("cp", &Attack->ObjectTransform.Position, vec2f(500, 100), vec2f(0, 0), false, true));
		Attack->ObjectTransform.Position.X -= 140;
		k = (Collider*)Attack->GetComponent("cp", "COLLIDER");
		k->velocity.X = -140;


		fireAttackCollisionL = false;
		attackCollisionOn = true;
	}

	// create and show colider on right
	if (fireAttackCollisionR)
	{
		Attack = this->Scene->AddObject(myAttackName);
		AttackAdvance->Layer = "rufusBasicAttack";
		Attack->ObjectTransform.Position = me->ObjectTransform.Position;
		Attack->AddComponent(&Collider("cp", &Attack->ObjectTransform.Position, vec2f(500, 100), vec2f(0, 0), false, true));
		Attack->ObjectTransform.Position.X -= 140;
		k = (Collider*)Attack->GetComponent("cp", "COLLIDER");
		k->velocity.X = -140;


		fireAttackCollisionR = false;
		attackCollisionOn = true;
	}

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
				if (player->ObjectTransform.Position.X  < me->ObjectTransform.Position.X)
				{
					me->ObjectTransform.Scale.X = -1;

					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "rufus attack basic"));
					animatorEnemy->SwitchClip("Rufus basicaAttack clip");
					showColiderAfter = true;
					isAttackWaiting = true;



				}
				//player in attack range from right
				else
				{
					me->ObjectTransform.Scale.X = 1;

					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "rufus attack basic"));
					animatorEnemy->SwitchClip("Rufus basicaAttack clip");
					showColiderAfterR = true;
					isAttackWaiting = true;

				}
			}
			//Player in see range from left
			else if (player->ObjectTransform.Position.X < me->ObjectTransform.Position.X)
			{
				int randomNum;
				randomNum = std::rand() % 1000;
				//move to left
				if (randomNum > 100){
					me->ObjectTransform.Scale.X = -1;
					me->ObjectTransform.Position.X -= 3;
					
					k = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
					k->velocity.X = -3;
					animatorEnemy->SwitchClip("Rufus walk clip");
					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources, "rufus walk"));
				}
				//or use advance attack
				else
				{
					me->ObjectTransform.Scale.X = -1;
					animatorEnemy->SwitchClip("Rufus AdvanceAttack clip");
					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAdvanceAname, &GetTexture(Resources, "rufus attack advance"));
					isAttackWaiting = true;
					showColiderAfterAdvance = true;
				}
			}
			//Player in see range from right
			else
			{
				int randomNum;
				randomNum = std::rand() % 1000;
				//move to left
				if (randomNum > 100){
					me->ObjectTransform.Scale.X = 1;
					me->ObjectTransform.Position.X += 3;
					k = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
					k->velocity.X = -140;
					//Scene->CollisionManager.GetCollider(myName)->velocity.X = 3;
					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources, "rufus walk"));
					animatorEnemy->SwitchClip("Rufus walk clip");
				}
				else{
					me->ObjectTransform.Scale.X = 1;

					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAdvanceAname, &GetTexture(Resources, "rufus attack advance"));
					animatorEnemy->SwitchClip("warrior walk clip");
					showColiderAfterAdvanceR = true;
					isAttackWaiting = true;
				}
			}

		}
		//Player not in see range
		else
		{
			animatorEnemy->SwitchClip("Rufus idle clip");
		//	Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myIdleAName, &GetTexture(Resources, "rufus idle"));
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
		s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

		EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));

	}


	if (holdColide == false){
		Collider *ownerCollider = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
		for (int i = 0; i < ownerCollider->checkObjectCollision().size(); i++){
			//std::cout << "collided names    " << sceneManager->CollisionManager.checkActorCollision(myName)[i] << std::endl;
			if ((ownerCollider->checkObjectCollision())[i] == "playerBasicAttack"){
				// do......
				me->ObjectTransform.Position.X = me->ObjectTransform.Position.X
					- 50 * me->ObjectTransform.Scale.X;

				myHp -= 15;
				holdColide = true;
				//EditSprite(&Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Buffers, vec3f(), Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Size, (1, .5, .5, 1));
				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.8));

			}
			else if ((ownerCollider->checkObjectCollision())[i] == "straightMonster"){
				// do......
				/*Scene->ActorManager.GetTransform(myName)->Position.X = Scene->ActorManager.GetTransform(myName)->Position.X
				- 100 * Scene->ActorManager.GetTransform(myName)->Scale.X;*/

				myHp -= 15;
				holdColide = true;
				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.8));

			}
			else if ((ownerCollider->checkObjectCollision())[i] == "ArcMonster"){
				// do......
				myHp -= 15;
				holdColide = true;
				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.8));

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