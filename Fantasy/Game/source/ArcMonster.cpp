#include <ArcMonster.h>

ArcMonster::ArcMonster(char *name, Game_Resources *resources)
{
	this->Name = name;
	this->Type = "ArcMonster";
	//this->SpriteData = sprite;
	this->Size = sizeof(ArcMonster);
	this->Resources = resources;
}

void ArcMonster::Init()
{
	AddTexture(Resources, LoadTexture("resources\\textures\\yellow.png"), "ArcMonster");

}

void ArcMonster::Update(float delta, Game_Input *input)
{
	if (this->Scene->GetGameObject("ArcMonster")!=NULL&& arcMon_hideFlag == true)
	{
		this->Scene->RemoveObject("ArcMonster");
		//sceneManager->ActorManager.DestroyActor(ActorName);
	}
	if (waitArcMon_falg == true && waitArcMon > 0)
		waitArcMon--;
	if (waitArcMon == 0)
	{
		waitArcMon_falg = false;
		waitArcMon = 121;

	}
	Animator *animatorPlayer = (Animator *)Owner->GetComponent("animator", "ANIMATOR");

	if (input->Buttons[Scene->GetInputKey('A')].KeyDown && waitArcMon_falg == false)
	{
		animatorPlayer->SwitchClip("noe summon clip");
		animatorPlayer->StopWhenDone();
		startArcMon = true;
		//sceneManager->AnimationManager.SwitchAnimation(myName, RenderableType::Movable, "noe summon clip", &GetTexture(gameResource, "noe summon"));
	
		//Game_Scene.RendererManager.GetRenderable(ActorName)->Skip = false;
		//myHp -= 10;
	}
	playerCollide *PlayerCollide = (playerCollide *)Owner->GetComponent("checkCollide", "player collide");

	if (startArcMon == true)
	{
		if ((animatorPlayer->GetCurrentClipName() == "noe summon clip"
			|| animatorPlayer->GetLastClipName() == "noe summon clip"))
		{
			if (!animatorPlayer->IsRunning("noe summon clip"))
			{
				arcMon_hideFlag = false;
				PlayerCollide->playerHp -= 20;
				arcMonster = this->Scene->AddObject("ArcMonster");
				arcMonster->ObjectTransform.Position = Owner->ObjectTransform.Position;
				arcMonster->AddComponent(&SpriteRenderer("ArcMonster_sprite", Sprite{ vec3f(0, 0, 0), vec2f(300*2.5f, 266*1.5f), Material{ GetTexture(Resources, "arc_Mon"), vec4f(1, 1, 1, 1) } }));
				arcMonster->ObjectTransform.Position.X = Owner->ObjectTransform.Position.X + 50 * Owner->ObjectTransform.Scale.X;
				arcMonster->ObjectTransform.Position.Y = Owner->ObjectTransform.Position.Y;
				arcMonster->ObjectTransform.Scale.X = Owner->ObjectTransform.Scale.X;
				x_ArcVelo = sin(arcMonDirection*PI / 180)*arcMonStrenght;
				y_ArcVelo = cos(arcMonDirection*PI / 180)*arcMonStrenght;
				diro = Owner->ObjectTransform.Scale.X;
				arcMonster->AddComponent(&Collider("arc", &arcMonster->ObjectTransform.Position, vec2f(300, 266), vec2f(0, 0), 0, true));
				arcMonster->AddComponent(&Animator("animator", (SpriteRenderer *)arcMonster->GetComponent("ArcMonster_sprite", "RENDERABLE")));
				//	Game_Scene.ActorManager.GetTransform("straight_Mon")->Position = vec3f(200.0f, -200.0f, 0);
				Animator *animator = (Animator *)arcMonster->GetComponent("animator", "ANIMATOR");
				animator->AddClip("arc monster clip", &AnimationClip(GetTexture(Resources, "arc_Mon"), 4, 5, (1.0f / 60.0f)*2.0f, true));

				arcMon_flag = true;
				waitArcMon_falg = true;
				startArcMon = false;
				
			}
		}
	}
	if (arcMon_flag == true && arcMonCount <80)
	{

		arcMonster->ObjectTransform.Position.X += x_ArcVelo*diro;
		arcMonster->ObjectTransform.Position.Y += y_ArcVelo;
		Collider *collider = (Collider *)arcMonster->GetComponent("arc", "COLLIDER");
		collider->velocity.X = x_ArcVelo*diro;
		collider->velocity.Y = y_ArcVelo;
		//sceneManager->CollisionManager.GetCollider(ActorName)->velocity.X = x_ArcVelo*diro;
		//sceneManager->CollisionManager.GetCollider(ActorName)->velocity.Y = y_ArcVelo;
		y_ArcVelo--;
		arcMonCount++;
		if (arcMonCount > 78)
		{
			arcMon_flag = false;
			arcMonCount = 0;
			arcMon_hideFlag = true;
			this->Scene->RemoveObject("ArcMonster");

		}
	}


}