#include <Collider.h>
#include <SceneManager.h>

std::vector <char*> Collider::checkObjectCollision()
{
	std::vector<char*> collidedActors;

	for (uint32 i = 0; i < Scene->PEngine.Colliders.size(); i++)
	{
		if (Scene->PEngine.Colliders[i]->Name1 == Name)
		{
			collidedActors.push_back(Scene->PEngine.Colliders[i]->Name2);
		}
		else if (Scene->PEngine.Colliders[i]->Name2 == Owner->Name)

		{
			collidedActors.push_back(Scene->PEngine.Colliders[i]->Name1);
		}
	}

	return collidedActors;
}

bool Collider::CheckGroundCollision()
{
	std::vector<GameComponent *> *colliders = Scene->GetAllComponents("COLLIDER");
	
	if (colliders)
	{
		down = false;
		jump = false;

		for (uint32 i = 0; i < colliders->size(); i++)
		{
			Collider *collider = (Collider*)(*colliders)[i];
			if (collider != this)
			{
				if (collider->ground)
				{
					if (Scene->PEngine.AABBvsAABB(this, collider))
					{
						//check down collision
						if (down == true)
						{
							jump = true;

							return true;
						}
					}
				}
			}
		}
	}	

	return false;
}

void Collider::Update(float delta, Game_Input *input)
{
	if (!ground)
	{
		CheckGroundCollision();
	}
}