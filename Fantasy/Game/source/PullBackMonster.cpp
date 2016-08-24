#include <PullBackMonster.h>

PullBackMonster::PullBackMonster(char *name, Game_Resources *resources)
{
	this->Name = name;
	this->Type = "PullBackMonster";
	//this->SpriteData = sprite;
	this->Size = sizeof(PullBackMonster);
	this->Resources = resources;
}

void PullBackMonster::Init()
{
	AddTexture(Resources, LoadTexture("resources\\textures\\yellow.png"), "ArcMonster");

}

void PullBackMonster::Update(float delta, Game_Input *input)
{
	if (pullBackHide_flag == true)
	{
		this->Scene->RemoveObject("pullBackMonster");
		//sceneManager->ActorManager.DestroyActor(ActorName);
	}
	if (waitPull_falg == true && waitPull > 0)
		waitPull--;
	if (waitPull == 0)
	{
		waitPull_falg = false;
		waitPull = 121;

	}
	Animator *animatorPlayer = (Animator *)Owner->GetComponent("animator", "ANIMATOR");
	playerCollide *PlayerCollide = (playerCollide *)Owner->GetComponent("checkCollide", "player collide");

	if (input->Buttons[Scene->GetInputKey('Z')].KeyDown  && waitPull_falg == false)
	{
		animatorPlayer->SwitchClip("noe summon clip");
		animatorPlayer->StopWhenDone();
		pullBackHide_flag = false;
		PlayerCollide->playerHp -= 20;
		pullBackMonster = this->Scene->AddObject("pullBackMonster");
		pullBackMonster->ObjectTransform.Position = Owner->ObjectTransform.Position;
		pullBackMonster->ObjectTransform.Position.X -= 200.0f*Owner->ObjectTransform.Scale.X;
		pullBackMonster->AddComponent(&SpriteRenderer("PullBackMonster_sprite", Sprite{ vec3f(0, 0, 0), vec2f(300, 266), Material{ GetTexture(Resources, "hook"), vec4f(1, 1, 1, 1) } }));
		pullBackMonster->AddComponent(&Collider("pullBack", &pullBackMonster->ObjectTransform.Position, vec2f(300, 266), vec2f(0, 0), 0, true));
		pullBackMonster->AddComponent(&Animator("animator", (SpriteRenderer *)pullBackMonster->GetComponent("PullBackMonster_sprite", "RENDERABLE")));
		//	Game_Scene.ActorManager.GetTransform("straight_Mon")->Position = vec3f(200.0f, -200.0f, 0);
		
		Animator *animator = (Animator *)pullBackMonster->GetComponent("animator", "ANIMATOR");
		animator->AddClip("pullBack monster clip", &AnimationClip(GetTexture(Resources, "hook"), 4, 5, (1.0f / 60.0f), true));
		animator->StopWhenDone();
		//sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe summon clip", &GetTexture(gameResource, "noe summon"));
		startPullBackMon = true;
		

	}
	if (startPullBackMon == true)
	{
		if ((animatorPlayer->GetCurrentClipName() == "noe summon clip"
			|| animatorPlayer->GetLastClipName() == "noe summon clip"))
		{
			if (!animatorPlayer->IsRunning("noe summon clip"))
			{
				pullBack_flag = true;
				
			
				startPullBackMon = true;
				waitPull_falg = true;
			}
		}
	}
	if (pullBack_flag == true && pullCount<9)
	{
		

		Owner->ObjectTransform.Position.X -= (40.0f* Owner->ObjectTransform.Scale.X);
		Collider *collider = (Collider *)Owner->GetComponent("c1", "COLLIDER");
		collider->velocity.X = -40.0f* Owner->ObjectTransform.Scale.X;
		//sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = -40.0f* sceneManager->ActorManager.GetTransform(ActorName)->Scale.X;

		pullCount++;
		if (pullCount >7)
		{
			pullBack_flag = false;
			pullCount = 0;
			pullBackHide_flag = true;
			this->Scene->RemoveObject("pullBackMonster");
		}
	}

}