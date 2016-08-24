#include <BasicAttack.h>

BasicAttack::BasicAttack(char *name, Game_Resources *resources)
{
	this->Name = name;
	this->Type = "BasicAttack";
	//this->SpriteData = sprite;
	this->Size = sizeof(BasicAttack);
	this->Resources = resources;
}

void BasicAttack::Init()
{
	AddTexture(Resources, LoadTexture("resources\\textures\\yellow.png"), "ArcMonster");

}

void BasicAttack::Update(float delta, Game_Input *input)
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

	if (attackCollisionOn == true)
	{

		attackCollisionOn = false;
		this->Scene->RemoveObject("playerBasicAttack");
	}
	if (fireAttackCollisionL)
	{
		basicAttack = this->Scene->AddObject("playerBasicAttack");
		basicAttack->ObjectTransform.Position = Owner->ObjectTransform.Position;
		//basicAttack->AddComponent(&SpriteRenderer("basicAttack_sprite", Sprite{ vec3f(0, 0, 0), vec2f(50, 100), Material{ GetTexture(Resources, "ArcMonster"), vec4f(1, 1, 1, 1) } }));
		basicAttack->AddComponent(&Collider("basic", &basicAttack->ObjectTransform.Position, vec2f(10, 200), vec2f(0, 0), 0, false));

		basicAttack->ObjectTransform.Position.X -= 340;
		//sceneManager->CollisionManager.GetCollider("playerBasicAttack")->velocity.X = 140;
		Collider *collider = (Collider *)basicAttack->GetComponent("basic", "COLLIDER");
		collider->velocity.X = -340;


		fireAttackCollisionL = false;
		attackCollisionOn = true;
	}


	if (fireAttackCollisionR)
	{
		basicAttack = this->Scene->AddObject("playerBasicAttack");
		basicAttack->ObjectTransform.Position = Owner->ObjectTransform.Position;
		//basicAttack->AddComponent(&SpriteRenderer("basicAttack_sprite", Sprite{ vec3f(0, 0, 0), vec2f(50, 100), Material{ GetTexture(Resources, "ArcMonster"), vec4f(1, 1, 1, 1) } }));
		basicAttack->AddComponent(&Collider("basic", &basicAttack->ObjectTransform.Position, vec2f(10, 200), vec2f(0, 0), 0, true));

		basicAttack->ObjectTransform.Position.X += 340;
		//sceneManager->CollisionManager.GetCollider("playerBasicAttack")->velocity.X = 140;
		Collider *collider = (Collider *)basicAttack->GetComponent("basic", "COLLIDER");
		collider->velocity.X = 340;

		fireAttackCollisionR = false;
		attackCollisionOn = true;
	}

	Animator *animatorPlayer = (Animator *)Owner->GetComponent("animator", "ANIMATOR");

	if (isAttackWaiting)
	{

	}
	else
	{
		if (input->Buttons[Scene->GetInputKey('D')].KeyDown  && Owner->ObjectTransform.Scale.X<0)
		{
			animatorPlayer->SwitchClip("noe basicattack clip");
			animatorPlayer->StopWhenDone();
			//sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe attack clip", &GetTexture(gameResource, "noe basicattack"));

			//Owner->ObjectTransform.Position.Y = Owner->ObjectTransform.Position.Y + 120;
			showColiderAfter = true;
			isAttackWaiting = true;


		}
		else if (input->Buttons[Scene->GetInputKey('D')].KeyDown  && Owner->ObjectTransform.Scale.X>0)
		{
			//sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe attack clip", &GetTexture(gameResource, "noe basicattack"));
			animatorPlayer->SwitchClip("noe basicattack clip");
			animatorPlayer->StopWhenDone();
			//Owner->ObjectTransform.Position.Y = Owner->ObjectTransform.Position.Y + 120;
			showColiderAfterR = true;
			isAttackWaiting = true;

		}
	}
}
