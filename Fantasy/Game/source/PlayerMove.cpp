#include<PlayerMove.h>

PlayerMove::PlayerMove(char *name, char * colliderName, float goalVelocity, bool Run, char *type )
{
	this->Name = name;
	this->Type = type;
	this->colliderName = colliderName;
	this->goalVelocity = goalVelocity;
	this->Run = Run;
	this->Size = sizeof(PlayerMove);
}


void PlayerMove:: Init()
{
	currentvelocity = 0;
	

}

void PlayerMove::Update(float delta, Game_Input *input)
{

	Collider *collider = (Collider *)Owner->GetComponent(colliderName, "COLLIDER");
	currentvelocity = collider->velocity.X;

	if ((!input->RIGHT.KeyDown) && (!input->LEFT.KeyDown)/*!input->LEFT.KeyDown&&!input->RIGHT.KeyDown*/){
		collider->velocity.X = 0.0f;
		animator = (Animator *)Owner->GetComponent("animator", "ANIMATOR");
		currentvelocity = 0.0f;
		if ((!input->A.KeyDown) && (!input->S.KeyDown) && (!input->D.KeyDown) && (!input->W.KeyDown) && (!input->Space.KeyDown))
		{
			if (animator->GetCurrentClipName() != "noe walk clip" && animator->GetCurrentClipName() != "noe idle clip")
			{
				animator->StopWhenDone();
				if (!animator->IsRunning(animator->GetCurrentClipName()))
				{
					animator->SwitchClip("noe idle clip");
				}
			}
			else
			{
				animator->SwitchClip("noe idle clip");
			}
		}
	}
	else if (input->RIGHT.KeyDown)
	{
		if (collider->rigth == false)
		{
			//currentvelocity.X = Accelerate(goalVelocity.X, currentvelocity.X, delta);
			currentvelocity = goalVelocity*delta;
			animator = (Animator *)Owner->GetComponent("animator", "ANIMATOR");

			if (animator->GetCurrentClipName() != "noe walk clip" && animator->GetCurrentClipName() != "noe idle clip")
			{
				animator->StopWhenDone();
				if (!animator->IsRunning((animator->GetCurrentClipName())))
				{
					animator->SwitchClip("noe walk clip");
				}
			}
			else
			{
				animator->SwitchClip("noe walk clip");
			}

			Owner->ObjectTransform.Position.X += currentvelocity;
			Owner->ObjectTransform.Scale.X = 1;
			collider->velocity.X = currentvelocity;
		}
	}
	else if (input->LEFT.KeyDown)
	{
		if (collider->left == false)
		{
			//currentvelocity.X = -1.0f * Accelerate(goalVelocity.X, currentvelocity.X, delta);

			currentvelocity = -1.0f*goalVelocity*delta;
			animator = (Animator *)Owner->GetComponent("animator", "ANIMATOR");
			if (animator->GetCurrentClipName() != "noe walk clip" && animator->GetCurrentClipName() != "noe idle clip")
			{
				animator->StopWhenDone();
				if (!animator->IsRunning((animator->GetCurrentClipName())))
				{
					animator->SwitchClip("noe walk clip");
				}
			}
			else
			{
				animator->SwitchClip("noe walk clip");
			}

			Owner->ObjectTransform.Position.X += currentvelocity;

			Owner->ObjectTransform.Scale.X = -1;

			collider->velocity.X = currentvelocity;
		}
	}

	AutoMoveCameraHorizontal(&Owner->Scene->MainCamera, currentvelocity, Owner->ObjectTransform.Position, collider->size);
	//Owner->Scene->MainCamera.Eye = Owner->ObjectTransform.Position;

}