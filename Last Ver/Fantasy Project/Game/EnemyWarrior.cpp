#include "EnemyWarrior.h"
#include <stdlib.h>


EnemyWarrior::EnemyWarrior(SceneManager *scene, Game_Resources *resources, char* name, vec3f pos, vec2f size, char* textureName, char* attackName, char* hbBarName, char* idleAName, char* walkAname, char* attackAName)
{
	//fill my inner varaibles
	Resources = resources;
	Scene = scene;
	myName = name;
	myAttackName = attackName;
	myHbBarName = hbBarName;
	myIdleAName = idleAName;
	myWalkAName = walkAname;
	myAttackAName = attackAName;

	//create warrior actor and it's component
	Scene->ActorManager.AddActor(name, Transform{});
	Scene->ActorManager.GetTransform(name)->Position = pos;
	Scene->RendererManager.AddComponent(name, RenderableType::Movable, vec3f(), size, Material{ GetTexture(Resources, textureName), vec4f{ 1, 1, 1, 1 } },Scene->AnimationManager.GetAnimationClip(idleAName));
	Scene->CollisionManager.AddComponent(name, &Scene->ActorManager.GetTransform(name)->Position, vec2f(150,300), false, false, true);

	//create warrior HP Bar actor and it's component
	Scene->ActorManager.AddActor(hbBarName, Transform{});
	Scene->ActorManager.GetTransform(hbBarName)->Position = pos;
	Scene->ActorManager.GetTransform(hbBarName)->Position.Y += 200;
	Scene->RendererManager.AddComponent(hbBarName, RenderableType::Movable, vec3f(), vec2f(150,25), Material{ GetTexture(Resources, "Empty"), vec4f{ 0, 1, .5, 1 } });

}

void EnemyWarrior::Update()
{
	//my HP follow me 
	Scene->ActorManager.GetTransform(myHbBarName)->Position.X = Scene->ActorManager.GetTransform(myName)->Position.X;
	Scene->ActorManager.GetTransform(myHbBarName)->Position.Y = Scene->ActorManager.GetTransform(myName)->Position.Y+250;
	//update my Hp
	Scene->ActorManager.GetTransform(myHbBarName)->Scale.X=(myHp/100.0f)*1.0f;


	//timer for make the colider of attack wait a little then appear on left
	if (showColiderAfter){ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionL = true; showColiderAfter = false; showColiderAfterTimer = 0; }
	//timer for make the colider of attack wait a little then appear on right
	if (showColiderAfterR){ showColiderAfterTimer++; }
	if (showColiderAfterTimer == 35){ fireAttackCollisionR = true; showColiderAfterR = false; showColiderAfterTimer = 0; }

	//timer for make the enemy wait a little after an attack
	
	if (isAttackWaiting){ waitAttackTimer++; }

	if (waitAttackTimer == waitAttackTime){ isAttackWaiting = false; waitAttackTimer = 0;  }
	//destroy colider after timer count
	if (attackCollisionOn){ attackCollisionOn = false; Scene->ActorManager.DestroyActor(myAttackName); }
	// create and show colider on left
	if (fireAttackCollisionL) 
	{
		Scene->ActorManager.AddActor(myAttackName, Transform{});
		Scene->ActorManager.GetTransform(myAttackName)->Position = Scene->ActorManager.GetTransform(myName)->Position;

		Scene->RendererManager.AddComponent(myAttackName, RenderableType::Movable, vec3f(), vec2f(50, 100), Material{ GetTexture(Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });
		Scene->CollisionManager.AddComponent(myAttackName, &Scene->ActorManager.GetTransform(myAttackName)->Position, vec2f(10, 100), false, false, true);
		
			Scene->ActorManager.GetTransform(myAttackName)->Position.X -= 140;
			Scene->CollisionManager.GetCollider(myAttackName)->velocity.X = -140;
		
		
		fireAttackCollisionL = false;
		attackCollisionOn = true;
	}

	// create and show colider on right
	if (fireAttackCollisionR)
	{
		Scene->ActorManager.AddActor(myAttackName, Transform{});
		Scene->ActorManager.GetTransform(myAttackName)->Position = Scene->ActorManager.GetTransform(myName)->Position;

		Scene->RendererManager.AddComponent(myAttackName, RenderableType::Movable, vec3f(), vec2f(50, 100), Material{ GetTexture(Resources, "Empty"), vec4f{ 1, 1, 1, 0 } });
		Scene->CollisionManager.AddComponent(myAttackName, &Scene->ActorManager.GetTransform(myAttackName)->Position, vec2f(10, 100), false, false, true);
		
			Scene->ActorManager.GetTransform(myAttackName)->Position.X += 140;
			Scene->CollisionManager.GetCollider(myAttackName)->velocity.X = 140;
		

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
		if (std::abs(Scene->ActorManager.GetTransform("player")->Position.X - Scene->ActorManager.GetTransform(myName)->Position.X) < seeRange)
		{
			//if the player in Attack range 
			if (std::abs(Scene->ActorManager.GetTransform("player")->Position.X - Scene->ActorManager.GetTransform(myName)->Position.X) < attackRange)
			{
				//player in attack range from left
				if (Scene->ActorManager.GetTransform("player")->Position.X < Scene->ActorManager.GetTransform(myName)->Position.X)
				{
					Scene->ActorManager.GetTransform(myName)->Scale.X = -10;
					Scene->ActorManager.GetTransform(myName)->Scale.Y = 1.4;
					Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "wAttack"));
					Scene->ActorManager.GetTransform(myName)->Scale.X = -1;
					showColiderAfter = true;
					isAttackWaiting = true;
					

					
				}
				//player in attack range from right
				else
				{
					Scene->ActorManager.GetTransform(myName)->Scale.X = 10;
					Scene->ActorManager.GetTransform(myName)->Scale.Y = 1.4;
					Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myAttackAName, &GetTexture(Resources, "wAttack"));
					Scene->ActorManager.GetTransform(myName)->Scale.X = 1;
					showColiderAfterR = true;
					isAttackWaiting = true;
					
				}
			}
			//Player in see range from left
			else if (Scene->ActorManager.GetTransform("player")->Position.X < Scene->ActorManager.GetTransform(myName)->Position.X)
			{
				Scene->ActorManager.GetTransform(myName)->Scale.X = -1;
				Scene->ActorManager.GetTransform(myName)->Position.X -= 3;
				Scene->CollisionManager.GetCollider(myName)->velocity.X = -3;


				Scene->ActorManager.GetTransform(myName)->Scale.X = -1.3;
				Scene->ActorManager.GetTransform(myName)->Scale.Y = 1;
				Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources,"warrior walk"));

			}
			//Player in see range from right
			else
			{
				Scene->ActorManager.GetTransform(myName)->Scale.X = 1;
				Scene->ActorManager.GetTransform(myName)->Position.X += 3;
				Scene->CollisionManager.GetCollider(myName)->velocity.X = 3;


				Scene->ActorManager.GetTransform(myName)->Scale.X = 1.3;
				Scene->ActorManager.GetTransform(myName)->Scale.Y = 1;
				Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myWalkAName, &GetTexture(Resources, "warrior walk"));
			}

		}
		//Player not in see range
		else
		{
			if (Scene->ActorManager.GetTransform(myName)->Scale.X > 0){ Scene->ActorManager.GetTransform(myName)->Scale.X = 1; }
			else{ Scene->ActorManager.GetTransform(myName)->Scale.X = -1; }
			Scene->ActorManager.GetTransform(myName)->Scale.Y = 1;
			Scene->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, myIdleAName, &GetTexture(Resources, "warrior idle"));
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
		Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->RenderableMaterial.MeshColor = vec4f(1, 1,1, 1);
	}


	if (holdColide == false){
		for (int i = 0; i < Scene->CollisionManager.checkActorCollision(myName).size(); i++){
			//std::cout << "collided names    " << sceneManager->CollisionManager.checkActorCollision(myName)[i] << std::endl;
			if (Scene->CollisionManager.checkActorCollision(myName)[i] == "playerBasicAttack"){
				// do......
				Scene->ActorManager.GetTransform(myName)->Position.X = Scene->ActorManager.GetTransform(myName)->Position.X
					- 50 * Scene->ActorManager.GetTransform(myName)->Scale.X;

				myHp -= 15;
				holdColide = true;
				//EditSprite(&Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Buffers, vec3f(), Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Size, (1, .5, .5, 1));
				Scene->RendererManager.GetRenderable(myName,RenderableType::Movable)->RenderableMaterial.MeshColor = vec4f(.1, .5, .5, 1);
			}
			else if (Scene->CollisionManager.checkActorCollision(myName)[i] == "straight_Mon"){
				// do......
				/*Scene->ActorManager.GetTransform(myName)->Position.X = Scene->ActorManager.GetTransform(myName)->Position.X
					- 100 * Scene->ActorManager.GetTransform(myName)->Scale.X;*/

				myHp -= 15;
				holdColide = true;
				EditSprite(&Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Buffers, vec3f(), Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Size, (1, .5, .5, 1));
			}
			else if (Scene->CollisionManager.checkActorCollision(myName)[i] == "arc_Mon"){
				// do......
				Scene->ActorManager.GetTransform(myName)->Position.X = Scene->ActorManager.GetTransform(myName)->Position.X
					- 100 * Scene->ActorManager.GetTransform(myName)->Scale.X;
				myHp -= 10;
				holdColide = true;
				EditSprite(&Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Buffers, vec3f(), Scene->RendererManager.GetRenderable(myName, RenderableType::Movable)->Size, (1, .5, .5, 1));
			}
		}
	}
	//if Hp reach 0 Destroy me
	if (myHp <= 0)
	{
		Scene->ActorManager.DestroyActor(myName);
		Scene->ActorManager.DestroyActor(myHbBarName);
	}
}



char* EnemyWarrior::GetName()
{
	return myName;
}
