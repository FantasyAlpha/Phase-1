#include<EnemyWizardMind.h>

EnemyWizardMind::EnemyWizardMind(char* comName, Game_Resources *resources, char* name, vec3f pos, char* attackName, char* hbBarName, char* idleAName, char* walkAname, char* attackAName, char* colliderName, char* spriteName)

{
	Resources = resources;

	myName = name;
	this->Type = "Wizard Mind";
	this->Name = comName;
	this->Size = sizeof(EnemyWizardMind);
	myAttackName = attackName;
	myHbBarName = hbBarName;
	myIdleAName = idleAName;
	myWalkAName = walkAname;
	myAttackAName = attackAName;
	myColliderName = colliderName;
	mySpriteName = spriteName;
	myPos = pos;

	//create warrior actor and it's component


	//create warrior HP Bar actor and it's component


}

void EnemyWizardMind::Init()
{
	player = this->Scene->GetGameObject("player");
	me = this->Scene->GetGameObject(myName);
	me->ObjectTransform.Position = myPos;

	HpBar = this->Scene->AddObject(myHbBarName);
	HpBar->ObjectTransform.Position = myPos;
	HpBar->ObjectTransform.Position.Y += 200;
	HpBar->AddComponent(&SpriteRenderer("HpBar_sprite", Sprite{ vec3f(0, 0, 0), vec2f(150, 25), Material{ GetTexture(Resources, "Empty"), vec4f(0, 1, .5, 1) } }));
}

void EnemyWizardMind::Update(float delta, Game_Input *input)
{//my HP follow me 
	HpBar->ObjectTransform.Position.X = me->ObjectTransform.Position.X;
	HpBar->ObjectTransform.Position.Y = me->ObjectTransform.Position.Y + 250;
	//update my Hp
	HpBar->ObjectTransform.Scale.X = (myHp / 100.0f)*1.0f;

	if (this->Scene->GetGameObject(myAttackName) != NULL){
		if (movingProjectile)
		{
			Attack->ObjectTransform.Position.X += stepX;
			Attack->ObjectTransform.Position.Y += stepY;

			movingProjectileTimer++;
		}
	}
	else{
		movingProjectileTimer = 0;
		movingProjectile = false;
	}

	if (movingProjectileTimer>50)
	{
		movingProjectileTimer = 0;
		movingProjectile = false;
		attackCollisionOn = true;
	}

	//timer for make the colider of attack wait a little then appear on left
	if (showColiderAfter)
	{ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionL = true; showColiderAfter = false; showColiderAfterTimer = 0; }
	//timer for make the colider of attack wait a little then appear on right
	if (showColiderAfterR){ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionR = true; showColiderAfterR = false; showColiderAfterTimer = 0; }

	//timer for make the enemy wait a little after an attack

	if (isAttackWaiting){ waitAttackTimer++; }

	if (waitAttackTimer == waitAttackTime){ isAttackWaiting = false; waitAttackTimer = 0; }
	//destroy colider after timer count
	if (attackCollisionOn)
	{
		attackCollisionOn = false;
		if (this->Scene->GetGameObject(myAttackName) != NULL){
			this->Scene->RemoveObject(myAttackName);
		}
	}
	// create and show colider on left
	if (fireAttackCollisionL)
	{
		/*Scene->ActorManager.AddActor(myAttackName, Transform{});
		Scene->ActorManager.GetTransform(myAttackName)->Position = Scene->ActorManager.GetTransform(myName)->Position;

		Scene->RendererManager.AddComponent(myAttackName, RenderableType::Movable, vec3f(), vec2f(100, 100), Material{ GetTexture(Resources, "wizard Projectile"), vec4f{ 1, 1, 1, 1 } }, Scene->AnimationManager.GetAnimationClip("wizard water attack animation clip"));
		Scene->CollisionManager.AddComponent(myAttackName, &Scene->ActorManager.GetTransform(myAttackName)->Position, vec2f(10, 100), false, false, true);
*/

		Attack = this->Scene->AddObject(myAttackName);
		Attack->Layer = "wizard";
		Attack->ObjectTransform.Position = me->ObjectTransform.Position;
		Attack->AddComponent(&SpriteRenderer("Wizard Projectile Sprite", Sprite{ vec3f(0, 0, 0), vec2f(200, 200), Material{ GetTexture(this->Resources, "wizard Projectile"), vec4f(1, 1, 1, 1) } }));
		Attack->AddComponent(&Animator("animatorWarriorEnemy_Attack", (SpriteRenderer *)Attack->GetComponent("Wizard Projectile Sprite", "RENDERABLE")));

		Animator *animatorAttack = (Animator *)Attack->GetComponent("animatorWarriorEnemy_Attack", "ANIMATOR");

		animatorAttack->AddClip("Attack clip", &AnimationClip(GetTexture(this->Resources, "wizard Projectile"), 2, 3, (1.0f / 60.0f)*2.0f, false));

		Attack->AddComponent(&Collider("cp", &Attack->ObjectTransform.Position, vec2f(10, 100), vec2f(0, 0), false, true));
		Attack->ObjectTransform.Position.X -= 140;
		Attack->ObjectTransform.Scale.X = -1;
		Collider* aCollider = (Collider*)Attack->GetComponent("cp", "COLLIDER");
		aCollider->velocity.X = -140;
		
		//Scene->ActorManager.GetTransform(myAttackName)->Scale.X = -1;
		/*Scene->ActorManager.GetTransform(myAttackName)->Position.X -= 140;
		Scene->CollisionManager.GetCollider(myAttackName)->velocity.X = -140;*/

		playerposx = player->ObjectTransform.Position.X;
		playerposy = player->ObjectTransform.Position.Y;
		stepX = (me->ObjectTransform.Position.X - playerposx) / 25;
		stepX *= -1;

		stepY = (me->ObjectTransform.Position.Y - playerposy) / 25;
		stepY *= -1;

		fireAttackCollisionL = false;
		//attackCollisionOn = true;
		movingProjectile = true;
	}

	// create and show colider on right
	if (fireAttackCollisionR)
	{
		
		Attack = this->Scene->AddObject(myAttackName);
		Attack->Layer = "wizard";
		Attack->ObjectTransform.Position = me->ObjectTransform.Position;
		Attack->AddComponent(&SpriteRenderer("Wizard Projectile Sprite", Sprite{ vec3f(0, 0, 0), vec2f(200, 200), Material{ GetTexture(this->Resources, "Wizard"), vec4f(1, 1, 1, 1) } }));
		Attack->AddComponent(&Collider("cp", &Attack->ObjectTransform.Position, vec2f(10, 100), vec2f(0, 0), false, true));
		Attack->AddComponent(&Animator("animatorWarriorEnemy_Attack", (SpriteRenderer *)Attack->GetComponent("Wizard Projectile Sprite", "RENDERABLE")));

		Animator *animatorAttack = (Animator *)Attack->GetComponent("animatorWarriorEnemy_Attack", "ANIMATOR");

		animatorAttack->AddClip("Attack clip", &AnimationClip(GetTexture(this->Resources, "wizard Projectile"), 2, 3, (1.0f / 60.0f)*2.0f, false));

		Attack->ObjectTransform.Scale.X = 1;
		Attack->ObjectTransform.Position.X += 140;
		Collider* aCollider = (Collider*)Attack->GetComponent("cp", "COLLIDER");
		aCollider->velocity.X = 140;


		playerposx = player->ObjectTransform.Position.X;
		playerposy = player->ObjectTransform.Position.Y;
		stepX = (me->ObjectTransform.Position.X - playerposx) / 25;
		stepX *= -1;

		stepY = (me->ObjectTransform.Position.Y - playerposy) / 25;
		stepY *= -1;

		fireAttackCollisionR = false;
		movingProjectile = true;
	}
	Animator *animatorEnemy = (Animator *)Owner->GetComponent("animatorWizardEnemy", "ANIMATOR");
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
				if (player->ObjectTransform.Position.X <  me->ObjectTransform.Position.X)
				{

					me->ObjectTransform.Scale.X = -1;
					//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "wizard attack"));
					animatorEnemy->SwitchClip("wizard basicattack clip");
					showColiderAfter = true;
					isAttackWaiting = true;



				}
				//player in attack range from right
				else
				{
					me->ObjectTransform.Scale.X = 1;
				//	Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "wizard attack"));
					animatorEnemy->SwitchClip("wizard basicattack clip");
					showColiderAfterR = true;
					isAttackWaiting = true;

				}
			}
			//Player in see range from left
			else if (player->ObjectTransform.Position.X <  me->ObjectTransform.Position.X)
			{

				me->ObjectTransform.Scale.X = -1;
				me->ObjectTransform.Position.X -= 1;
				k = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
				k->velocity.X = -1;
				//Scene->CollisionManager.GetCollider(myName)->velocity.X = -1;
				//Owner->GetComponent.GetCollider(myName)->velocity.X = -1;
				animatorEnemy->SwitchClip("wizard walk clip");

			//	Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources, "wizard walk"));

			}
			//Player in see range from right
			else
			{

				me->ObjectTransform.Scale.X = 1;
				me->ObjectTransform.Position.X += 1;
				k = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
				k->velocity.X = 1;

				animatorEnemy->SwitchClip("wizard walk clip");
				//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources, "wizard walk"));
			}

		}
		//Player not in see range
		else
		{
			animatorEnemy->SwitchClip("wizard idle clip");
			//Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myIdleAName, &GetTexture(Resources, "wizard idle"));
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
		ownerCollider = (Collider*)me->GetComponent(myColliderName, "COLLIDER");
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

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));

			}
			else if ((ownerCollider->checkObjectCollision())[i] == "straight_Mon"){
				// do......
				/*Scene->ActorManager.GetTransform(myName)->Position.X = Scene->ActorManager.GetTransform(myName)->Position.X
				- 100 * Scene->ActorManager.GetTransform(myName)->Scale.X;*/

				myHp -= 15;
				holdColide = true;
				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));

			}
			else if ((ownerCollider->checkObjectCollision())[i] == "arc_Mon"){
				// do......
				myHp -= 15;
				holdColide = true;
				s = (SpriteRenderer*)Owner->GetComponent(mySpriteName, "RENDERABLE");

				EditSprite(&s->SpriteData.Buffers, s->SpriteData.Pos, s->SpriteData.Size, vec4f(1, 1, 1, 0.3));

			}
		}
	}
	//if Hp reach 0 Destroy me
	if (myHp <= 0)
	{
		this->Scene->RemoveObject(myName);
		this->Scene->RemoveObject(myHbBarName);
	}

}



