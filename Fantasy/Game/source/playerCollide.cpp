#include<playerCollide.h>

playerCollide::playerCollide(char *name, Game_Resources *resources, char *myCollider)
{
	Resources = resources;
	this->MyCollider = myCollider;
	
	this->Type = "player collide";
	this->Name =name;
	this->Size = sizeof(playerCollide);
	


}

void playerCollide::Init()
{
	//player = this->Scene->GetGameObject("player");
	//me = this->Scene->GetGameObject(myName);
	//me->ObjectTransform.Position = myPos;

	mainCanvas = this->Scene->AddObject("main canvas");
	mainCanvas->ObjectTransform.Position = Owner->ObjectTransform.Position;
	mainCanvas->ObjectTransform.Position.Y += 600;
	mainCanvas->ObjectTransform.Position.X -= 1000;
	mainCanvas->AddComponent(&SpriteRenderer("mainCanvas", Sprite{ vec3f(0, 0, 0), vec2f(150 * 2.5f, 25 * 4.0f), Material{ GetTexture(Resources, "main Canvas"), vec4f(1, 1, 1, 1) } }));

	playerHpBar = this->Scene->AddObject("playerHpBar");
	playerHpBar->ObjectTransform.Position = Owner->ObjectTransform.Position;
	playerHpBar->ObjectTransform.Position.Y += 200;
	playerHpBar->AddComponent(&SpriteRenderer("playerHpBar_sprite", Sprite{ vec3f(0, 0, 0), vec2f(150 , 25), Material{ GetTexture(Resources, "Empty"), vec4f(0, 1, .5, 1) } }));

}

void playerCollide::Update(float delta, Game_Input *input)
{
	if (playerHp <= 0)
	{
		playerHpBar->ObjectTransform.Scale.X =1.0f;
	}

	mainCanvas->ObjectTransform.Position = Owner->ObjectTransform.Position;
	mainCanvas->ObjectTransform.Position.Y += 600;
	mainCanvas->ObjectTransform.Position.X -= 1000;
	playerHpBar->ObjectTransform.Position = Owner->ObjectTransform.Position;
	playerHpBar->ObjectTransform.Position.Y += 200;
	playerHpBar->ObjectTransform.Scale.X = (playerHp / 100.0f)*1.0f;
	collide = (Collider*)Owner->GetComponent(MyCollider, "COLLIDER");
	GameObject *object;
	for (int i = 0; i < collide->checkObjectCollision().size(); i++){
		std::cout << "en" << collide->checkObjectCollision()[i] << std::endl;
		//std::cout << "collided names    " << sceneManager->CollisionManager.checkActorCollision(myName)[i] << std::endl;
		object = this->Scene->GetGameObject(collide->checkObjectCollision()[i]);

		if (object)
		{
			if (object->Layer == "warrior"){
				// do......
				Owner->ObjectTransform.Position.X -= 100.0f*Owner->ObjectTransform.Scale.X;
				playerHp -= 5;
				//this->Scene->RemoveObject(collide->checkObjectCollision()[i]);
			}
			else if (object->Layer == "wizard")
			{

				Owner->ObjectTransform.Position.X -= 100.0f*Owner->ObjectTransform.Scale.X;
				playerHp -= 5;
				//this->Scene->RemoveObject(collide->checkObjectCollision()[i]);
			}
			else if (object->Layer == "rufusBasicAttack")
			{
				Owner->ObjectTransform.Position.X -= 100.0f*Owner->ObjectTransform.Scale.X;
				playerHp -= 10;
				//this->Scene->RemoveObject(collide->checkObjectCollision()[i]);
			}
			else if (object->Layer == "rufusAdvanceAttack")
			{
				Owner->ObjectTransform.Position.X -= 100.0f*Owner->ObjectTransform.Scale.X;
				playerHp -= 20;
				//this->Scene->RemoveObject(collide->checkObjectCollision()[i]);
			}
		}		
	}
}