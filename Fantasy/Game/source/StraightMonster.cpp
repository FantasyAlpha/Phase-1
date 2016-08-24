#include <StraightMonster.h>

StraightMonster::StraightMonster(char *name, Game_Resources *resources)
{
	this->Name = name;
	this->Type = "straightMonster";
	//this->SpriteData = sprite;
	this->Size = sizeof(StraightMonster);
	this->Resources = resources;
}

void StraightMonster::Init()
{
	AddTexture(Resources, LoadTexture("resources\\textures\\Genta.png"), "Genta");

}

void StraightMonster::Update(float delta, Game_Input *input)
{
	if (this->Scene->GetGameObject("straightMonster") != NULL&&straight_hideFlag == true)
	{
		this->Scene->RemoveObject("straightMonster");
		//this->Destroy();
		//Game_Scene.RendererManager.GetRenderable(ActorName)->Skip = true;
	}

	if (waitStraight_falg == true && waitStraight > 0)
		waitStraight--;
	if (waitStraight == 0)
	{
		waitStraight_falg = false;
		waitStraight = 121;

	}
	Animator *animatorPlayer = (Animator *)Owner->GetComponent("animator", "ANIMATOR");
	
	if (input->Buttons[Scene->GetInputKey('S')].KeyDown&& waitStraight_falg == false)
	{
		animatorPlayer->SwitchClip("noe summon clip");
		animatorPlayer->StopWhenDone();
		
		startStraightMon = true;

		//myHp -= 20;
	}
	playerCollide *PlayerCollide = (playerCollide *)Owner->GetComponent("checkCollide", "player collide");

	if (startStraightMon == true){
		if ((animatorPlayer->GetCurrentClipName() == "noe summon clip"
			|| animatorPlayer->GetLastClipName() == "noe summon clip"))
		{
			//animatorPlayer->StopWhenDone();
			if (!animatorPlayer->IsRunning("noe summon clip"))
			{
				straight_hideFlag = false;
				PlayerCollide->playerHp -= 20;
				straightMonster = this->Scene->AddObject("straightMonster");
				straightMonster->ObjectTransform.Position = Owner->ObjectTransform.Position;


				straightMonster->AddComponent(&SpriteRenderer("straightMonster_sprite", Sprite{ vec3f(0, 0, 0), vec2f(400, 266), Material{ GetTexture(Resources, "straight_Mon"), vec4f(1, 1, 1, 1) } }));
				straightMonster->ObjectTransform.Position.X = Owner->ObjectTransform.Position.X + 100 * Owner->ObjectTransform.Scale.X;
				straightMonster->ObjectTransform.Position.Y = Owner->ObjectTransform.Position.Y-100;
				straightMonster->ObjectTransform.Scale.X = Owner->ObjectTransform.Scale.X;
				
				straightMonster->AddComponent(&Collider("straightMon", &straightMonster->ObjectTransform.Position, vec2f(400, 266), vec2f(0, 0), 0, true));
				straightMonster->AddComponent(&Animator("animator", (SpriteRenderer *)straightMonster->GetComponent("straightMonster_sprite", "RENDERABLE")));
				//	Game_Scene.ActorManager.GetTransform("straight_Mon")->Position = vec3f(200.0f, -200.0f, 0);
				Animator *animator = (Animator *)straightMonster->GetComponent("animator", "ANIMATOR");
				animator->AddClip("straight monster clip", &AnimationClip(GetTexture(Resources, "straight_Mon"), 2, 5, 1.0f / 60.0f, true));
			
				straight_flag = true;
				waitStraight_falg = true;
				startStraightMon = false;
				

			}

		}
	}
	if (straight_flag == true && straightCount <80)
	{
		straightMonster->ObjectTransform.Position.X += 10.0f* straightMonster->ObjectTransform.Scale.X;
		Collider *collider = (Collider *)straightMonster->GetComponent("straightMon", "COLLIDER");
		collider->velocity.X = 10.0f*straightMonster->ObjectTransform.Scale.X;
		//sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = 10.0f*sceneManager->ActorManager.GetTransform(ActorName)->Scale.X;

		straightCount++;
		if (straightCount > 78)
		{
			straight_flag = false;
			straightCount = 0;
			straight_hideFlag = true;
			this->Scene->RemoveObject("straightMonster");

			//Game_Scene.ActorManager.DestroyActor("straight_Mon");
			//Game_Scene.RendererManager.RemoveRenderer("straight_Mon");
			//Game_Scene.CollisionManager.RemoveComponent("straight_Mon");
		}
	}

}