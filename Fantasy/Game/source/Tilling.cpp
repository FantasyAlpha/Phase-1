#include <Tilling.h>

Tilling::Tilling(char *name, Game_Resources *resources, char* name1, char* name2, vec3f pos, vec2f size, char* textureName, float screenWidth,float zaxis)

{
	this->Name = name;
	this->Type = "Tilling";
	//this->SpriteData = sprite;
	this->Size = sizeof(Tilling);
	this->Resources = resources;
	this->myPos = pos;
	this->mySize = size;
	this->textureName = textureName;
	this->backgroundActorName = name1;
	this->tilingActorName1 = name1;
	this->tilingActorName2 = name2;
	this->ScreenWidth = screenWidth;
	this->zAxis = zaxis;
}

void Tilling::Init()
{
	preCameraPos = this->Scene->MainCamera.Eye.X;
	this->Scene->GetGameObject(tilingActorName1);
	this->Scene->GetGameObject(tilingActorName1)->ObjectTransform.Position = myPos;
	this->Scene->GetGameObject(tilingActorName1)->AddComponent(&SpriteRenderer("tilling"
		, Sprite{ vec3f(0, 0, 0), mySize, Material{ GetTexture(Resources, textureName), vec4f(1, 1, 1, 1) } }));

	this->Scene->GetGameObject(tilingActorName2);
	this->Scene->GetGameObject(tilingActorName2)->ObjectTransform.Position = myPos;
	this->Scene->GetGameObject(tilingActorName2)->AddComponent(&SpriteRenderer("tilling", Sprite{ vec3f(0, 0, 0), mySize, Material{ GetTexture(Resources, textureName), vec4f(1, 1, 1, 1) } }));

}

void Tilling::Update(float delta, Game_Input *input)
{
	if (Scene->CurrentRoot)
	{
		if (this->Owner->CurrentRoot != Scene->CurrentRoot->Name)
		{
			return;
		}
	}
	
	//parallaxing
	parallax = (preCameraPos - this->Scene->MainCamera.Eye.X)*zAxis*-1;
	this->Scene->GetGameObject(tilingActorName1)->ObjectTransform.Position.X += parallax;
	this->Scene->GetGameObject(tilingActorName2)->ObjectTransform.Position.X += parallax;
	preCameraPos = this->Scene->MainCamera.Eye.X;
	//background tiling trial
	//forward tiling right
	float edgeVisibleRight = (this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Position.X + spriteWidth / 2.0f) - (ScreenWidth / 2.0f);
	//backword titling left
	float edgeVisibleLeft = (this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Position.X - spriteWidth / 2.0f) + (ScreenWidth / 2.0f);
	//which background in scene the check if it has right or left buddies
	if (backgroundActorName == tilingActorName1){
		hasRightBuddy = BG0_R;
		hasLeftBuddy = BG0_L;
	}
	else if (backgroundActorName == tilingActorName2){
		hasRightBuddy = BG1_R;
		hasLeftBuddy = BG1_L;
	}
	//not have right or left create new before seen by camera
	if (hasRightBuddy == false && this->Scene->MainCamera.Eye.X >= edgeVisibleRight &&createNewBackGround == false)
	{
		//std::cout << "enter" << std::endl;
		createNewBackGround = true;

		rightorleft = 1;


	}
	else if (hasLeftBuddy == false && this->Scene->MainCamera.Eye.X <= edgeVisibleLeft&&createNewBackGround == false)
	{
		createNewBackGround = true;
		rightorleft = -1;
	}
	// has right or left switch now background to the one on right <or one on left>
	else if (hasRightBuddy == true && this->Scene->MainCamera.Eye.X >= edgeVisibleRight &&createNewBackGround == false)
	{
		if (backgroundActorName == tilingActorName1) backgroundActorName = tilingActorName2;
		else if (backgroundActorName == tilingActorName2)backgroundActorName = tilingActorName1;

		rightorleft = 1;
	}
	else if (hasLeftBuddy == true && Scene->MainCamera.Eye.X <= edgeVisibleLeft&&createNewBackGround == false)
	{
		if (backgroundActorName == tilingActorName1) backgroundActorName = tilingActorName2;
		else if (backgroundActorName == tilingActorName2)backgroundActorName = tilingActorName1;
		rightorleft = -1;
	}
	// create new background according to rightorleft
	if (createNewBackGround == true)
	{
		
		previousBackGround = backgroundActorName;
		
		previousPosition = this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Position.X;
		//std::cout << "name  " << previousBackGround << "pos  "<<previousPosition<< std::endl;
		//switch between 2 backgrounds

		if (backgroundActorName == tilingActorName1) {
			backgroundActorName = tilingActorName2;
			this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Position.X = previousPosition + (ScreenWidth*2.5 +400.0f)* rightorleft;
			this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Scale.X = this->Scene->GetGameObject(previousBackGround)->ObjectTransform.Scale.X *-1;
			//std::cout << "name  " << backgroundActorName << "pos  " << this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Position.X << std::endl;

		}
		else if (backgroundActorName == tilingActorName2) {
			backgroundActorName = tilingActorName1;
			this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Position.X = previousPosition + (ScreenWidth*2.5+400.0f)* rightorleft;
			this->Scene->GetGameObject(backgroundActorName)->ObjectTransform.Scale.X = this->Scene->GetGameObject(previousBackGround)->ObjectTransform.Scale.X *-1;
		}


		if (rightorleft == 1)
		{
			if (backgroundActorName == tilingActorName1){
				BG0_L = true;
				BG0_R = false;
				BG1_R = true;
				BG1_L = false;
			}
			else if (backgroundActorName == tilingActorName2){
				BG1_L = true;
				BG1_R = false;
				BG0_R = true;
				BG0_L = false;
			}

		}
		else if (rightorleft == -1)
		{
			if (backgroundActorName == tilingActorName1){
				BG0_L = false;
				BG0_R = true;
				BG1_L = true;
				BG1_R = false;
			}
			else if (backgroundActorName == tilingActorName2){
				BG1_L = false;
				BG1_R = true;
				BG0_L = true;
				BG0_R = false;
			}
		}

		createNewBackGround = false;




	}
}