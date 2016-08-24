#include <ApplyGravity.h>

ApplyGravity::ApplyGravity(char *name, char * colliderName, char *type )
{
	this->Name = name;
	this->Type = type;
	this->colliderName = colliderName;
	this->Size = sizeof(ApplyGravity);
}

void ApplyGravity :: Init()
{

	this->fallSpeed = 0.0f;


}

void  ApplyGravity::Update(float delta, Game_Input *input)
{
	bool groundCheck = false;
	bool Jump;
	Collider *collider = (Collider *)Owner->GetComponent(colliderName, "COLLIDER");

	Jump = collider->jump;

	if (Jump)
	{

		groundCheck = true;

	}

	if (groundCheck)
	{
		//Jump = true;
		fallSpeed = 0;
		collider->velocity.Y = 0;

	}

	else if (!groundCheck)
	{

		fallSpeed += (collider->Scene->PEngine.ph.Gravity*delta*25.0f);

		Owner->ObjectTransform.Position.Y -= fallSpeed;
		//Game_Scene.CollisionManager.GetCollider(ActorName)->pos->Y -= fallSpeed;
		collider->velocity.Y = -fallSpeed;

	}
}