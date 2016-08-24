#include <SceneManager.h>
#include <PhysicsEngine.h>
#include <iostream>

void PhysicsEngine::Init()
{
	CollidedNamesPool = Cinder::Memory::MemoryPool(sizeof(CollidedPair), 8, false);
	ph.acceleration = 60.0f;
	ph.Gravity = 4.0f;
}

void PhysicsEngine::Update()
{
	std::vector<GameComponent *> *colliders = Scene->GetAllComponents("COLLIDER");

	if (colliders)
	{
		for (uint32 i = 0; i < Colliders.size(); i++)
		{
			CollidedNamesPool.Free(Colliders[i]);
		}

		Colliders.clear();

		for (uint32 i = 0; i < colliders->size(); i++)
		{
			Collider *collider = (Collider*)(*colliders)[i];

			if (Scene->CurrentRoot)
			{
				if (collider->Owner->CurrentRoot != Scene->CurrentRoot->Name)
				{
					continue;
				}
			}

			if (!collider->Owner->Enabled)
			{
				continue;
			}

			//collider->velocity = (*(collider->pos) - *(collider->OldPos)).XY();

			collider->rigth = false;
			collider->left = false;
			collider->down = false;
			collider->up = false;
		}

		for (uint32 i = 0; i < colliders->size(); i++)
		{
			for (uint32 j = 0; j < colliders->size(); j++)
			{
				if (i == j)
				{
					continue;
				}

				Collider *collider1 = (Collider*)(*colliders)[i];
				Collider *collider2 = (Collider*)(*colliders)[j];

				if (Scene->CurrentRoot)
				{
					if (collider1->Owner->CurrentRoot != Scene->CurrentRoot->Name
						|| collider2->Owner->CurrentRoot != Scene->CurrentRoot->Name)
					{
						continue;
					}
				}				

				if (!collider1->Owner->Enabled || !collider2->Owner->Enabled)
				{
					continue;
				}

				CollidedPair CollidedName = { collider1->Owner->Name, collider2->Owner->Name };

				if (SweptAABBvsAABB(collider1, collider2, Scene->Delta))
				{
					CollidedPair *cp = (CollidedPair *)CollidedNamesPool.Alloc();
					if (cp)
					{
						*cp = CollidedName;
						Colliders.push_back(cp);
					}
				}
			}
		}
	}
}

bool PhysicsEngine::SweptAABBvsAABB(Collider *A, Collider *B, float delta)
{
	vec2f d0, d1;
	vec2f VAB;
	vec2f u0, u1;
	float t0, t1;
	int count = 0;
	int flag = 0;
	VAB.X = (A->velocity.X - (B->velocity.X));
	VAB.Y = A->velocity.Y - (B->velocity.Y);

	bool temp = false;
	vec2f a_extent, b_extent;
	vec2f amin, amax, bmin, bmax;
	vec2f diffrence;

	a_extent.X = A->pos->X +A->offset.X+ (A->size.X) / 2.0f;
	b_extent.X = B->pos->X + B->offset.X + B->size.X / 2.0f;

	//diffrence.X = (b_extent.X - a_extent.X);
	diffrence.X = (B->pos->X + B->offset.X) - (A->pos->X + A->offset.X);

	a_extent.Y = A->pos->Y + A->offset.Y + (A->size.Y) / 2.0f;
	b_extent.Y = B->pos->Y + B->offset.Y + B->size.Y / 2.0f;

	//diffrence.Y = (b_extent.Y - a_extent.Y);
	diffrence.Y = B->pos->Y + B->offset.Y - (A->pos->Y + A->offset.Y);


	//d0.X = B->collider.pos.X - (A->collider.pos.X + A->collider.size.X);
	//d1.X = B->collider.pos.X + B->collider.size.X - A->collider.pos.X;

	d0.X = abs(diffrence.X) - (A->size.X / 2.0f + B->size.X / 2.0f);
	d0.Y = abs(diffrence.Y) - (A->size.Y / 2.0f + B->size.Y / 2.0f);

	d1.X = abs(diffrence.X) + (A->size.X / 2.0f + B->size.X / 2.0f);
	d1.Y = abs(diffrence.Y) + (A->size.Y / 2.0f + B->size.Y / 2.0f);

	float x_overlap = ((A->size.X) / 2.0f + (B->size.X / 2.0f)) - abs(diffrence.X);
	float y_overlap = (A->size.Y / 2.0f + (B->size.Y / 2.0f)) - abs(diffrence.Y);




	if (abs(A->velocity.X) > 0 && A->velocity.Y == 0)
	{

		if (y_overlap > 0)
		{
			/*if (A->velocity.X <= -10.0f){

			if (A->pos->X < B->pos->X){
			A->pos->X += A->velocity.X;
			if (A->pos->X > B->pos->X)
			{
			diffrence.X = B->pos->X - A->pos->X;
			d0.X = abs(diffrence.X) - (A->size.X / 2.0f + B->size.X / 2.0f);
			d1.X = abs(diffrence.X) + (A->size.X / 2.0f + B->size.X / 2.0f);

			u0.X = abs(d0.X / abs(VAB.X));
			u1.X = d1.X / abs(VAB.X);
			u0.Y = 0;
			u1.Y = 1.0f;
			}
			else {
			A->pos->X -= A->velocity.X;

			}
			}*/
			//}

			if (d0.X < 0) {
				u1.X = 1;
				u0.Y = 0;
				u1.Y = 1.0f;
				A->penetration = x_overlap;
				B->penetration = x_overlap;

				if (diffrence.X > 0) {

					A->normal = vec2f(1, 0);
					B->normal = vec2f(-1, 0);

				}

				else {
					A->normal = vec2f(-1, 0);
					B->normal = vec2f(1, 0);

				}
			}

			else
			{
				// compute eariler collsion time in x 
				u0.X = abs(d0.X / abs(VAB.X));
				//// compute latest collsion time in x 
				u1.X = d1.X / abs(VAB.X);
				u0.Y = 0;
				u1.Y = 1.0f;
			}

		}
		else {

			//std::cout << "Noo  collision " << std::endl;
			/*B->normal = vec2f(0, 0);
			A->normal = vec2f(0, 0);*/
			A->detected = 0;
			B->detected = 0;
			return false;
		}


	}


	else if (A->velocity.X == 0 && (abs(A->velocity.Y) > 0)) {

		if (x_overlap > 0)
		{
			if (d0.Y < 0) {
				u0.X = 0;
				u1.X = 1;
				u0.Y = 0;
				u1.Y = 1.0f;
				A->penetration = y_overlap;
				if (diffrence.Y>0) {
					A->normal = vec2f(0, 1);
				}
				else {
					A->normal = vec2f(0, -1);
				}
			}
			else
			{
				u0.Y = abs(d0.Y) / abs(VAB.X);
				u1.Y = abs(d1.Y) / abs(VAB.Y);

				u0.X = 0;
				u1.X = 1.0f;
				count = 2;
			}
		}
		else {
			//	std::cout << "Noo  collision " << std::endl;
			/*	B->normal = vec2f(0, 0);
			A->normal = vec2f(0, 0);*/
			A->detected = 0;
			B->detected = 0;

			return false;
		}
	}


	else if (abs(A->velocity.Y)> 0 && abs(A->velocity.X) > 0)
	{

		/*if (d0.X < 0 && d0.Y < 0) {
		u0.X = 0;
		u1.X = 1;
		u0.Y = 0;

		u1.Y = 1.0f;
		}
		else
		{*/
		// no checking here for the x , y overlaps cause we move in both directions 
		u0.X = abs(d0.X) / abs(VAB.X);
		u1.X = abs(d1.X) / abs(VAB.X);

		u0.Y = (d0.Y) / abs(VAB.Y);
		u1.Y = (d1.Y) / abs(VAB.Y);

		count = 4;

		/*}*/

	}

	//Standing away / sticking on one of the sides 

	else if (A->velocity.X == 0 && A->velocity.Y == 0)
	{

		/*if (d0.X < 0 && d0.Y < 0) {
		if (!A->ground &&!B->ground &&!A->wall &&!B->wall){
		u0.X = 0;
		u1.X = 1;
		u0.Y = 0;
		u1.Y = 1.0f;
		}
		}
		else
		{*/
		//std::cout << "Noo  collision " << std::endl;
		/*B->normal = vec2f(0, 0);
		A->normal = vec2f(0, 0);*/
		A->detected = 0;
		B->detected = 0;

		count = 1;

		return false;
	}
	//	}


	// **************************end of conditions start the real swept test to detect collision or Not ***********************

	t0 = max(u0.X, u0.Y);
	t1 = min(u1.X, u1.Y);

	//std::cout << "T0   "<<t0<<"   T1   " <<t1<< std::endl;

	if (t0 <= t1 && t0 >= 0 && t0 <= 1) {


		if (A->trigger == 1 || B->trigger == 1)
		{
			//	std::cout << "Trigger collision " << std::endl;
			if (!A->ground && !A->wall && !B->ground && !B->wall)
			{
				A->detected = true;
				B->detected = true;

				return true;
			}

		}
		//std::cout << "collision " << std::endl;
		if (u0.X == u0.Y){
			if (abs(A->velocity.X)> 0){
				flag = 1;
			}
			else if (abs(A->velocity.Y) > 0){
				flag = 2;
			}
		}
		if (u0.X > u0.Y || flag == 1)
		{

			// left & rigth

			if (A->wall)
			{
				A->jump = true;

			}
			else if (B->wall)
			{

				B->jump = true;

			}
			if (diffrence.X > 0)
			{

				// R A - L B 
				/*	A->normal = vec2f(1, 0);
				B->normal = vec2f(-1, 0);*/
				std::cout << "Rigth " << std::endl;
				//A->GlobalTransform.Position.X -= ((VAB.X)*delta - (d0.X));

				A->pos->X -= ((VAB.X)*.016f - (d0.X));

				A->rigth = 1;
				B->left = 1;

			}

			else if (diffrence.X <= 0)
			{
				/*A->normal = vec2f(-1, 0);
				B->normal = vec2f(1, 0);*/
				//	std::cout << "left " << std::endl;
				//A->GlobalTransform.Position.X += (abs(VAB.X)*delta - (d0.X));

				A->pos->X += (abs(VAB.X)*.016f - (d0.X));

				A->left = 1;
				B->rigth = 1;

			}

			//**********************importannnt
			A->velocity.X = 0.0f;
			//	B->velocity.X = 0.0f;

		}

		else if (u0.Y>u0.X || flag == 2) {

			if (diffrence.Y > 0)
			{

				/*A->normal = vec2f(0, 1);
				B->normal = vec2f(0, -1);*/
				//	std::cout << "up " << std::endl;

				// up A down B
				//A->GlobalTransform.Position.Y -= ((VAB.Y)*delta - (d0.Y));
				A->pos->Y -= ((VAB.Y)*.016f - (d0.Y));

				// no more jumping 
				A->up = 1;
				B->down = 1;

			}

			else  if (diffrence.Y <= 0)
			{

				/*	A->normal = vec2f(0, -1);
				B->normal = vec2f(0, 1);*/
				//	std::cout << "down " << std::endl;

				// down A UP B
				//fall
				//A->GlobalTransform.Position.Y += abs(VAB.Y)*.016f;
				A->pos->Y += abs(VAB.Y)*.016f;

				A->down = 1;
				B->up = 1;

			}

			//**********************importannnt
			A->velocity.Y = 0;
			//	B->velocity.Y = 0;


		}

		A->detected = true;
		B->detected = true;
		return true;

	}

	else
	{
		A->detected = false;
		B->detected = false;
		/*A->normal = vec2f(0, 0);
		B->normal = vec2f(0, 0);*/

		//std::cout << " disjoint  " << std::endl;

		return false;

	}
	

}
bool PhysicsEngine::AABBvsAABB(Collider *A, Collider *B)
{

	// Calculate half eXtents along X aXis for each object
	float a_eXtent = (A->pos->X + A->offset.X) + (A->size.X) / 2;
	float b_eXtent = (B->pos->X + B->offset.X) + B->size.X / 2;
	vec2f diffrence;
	//diffrence.X = (b_eXtent - a_eXtent);
	diffrence.X = (B->pos->X + B->offset.X) - (A->pos->X + A->offset.X);
	// Calculate overlap on X aXis
	float X_overlap = ((A->size.X) / 2 + (B->size.X / 2)) - abs(diffrence.X);
	//	std::cout << "X-overlap" <<X_overlap<< std::endl;
	// SAT test on X aXis

	if (X_overlap >= 0)
	{

		// Calculate half eXtents along X aXis for each object
		float a_eXtent = (A->pos->Y + A->offset.Y) + (A->size.Y / 2);
		float b_eXtent = (B->pos->Y + B->offset.Y) + (B->size.Y / 2);
		//diffrence.Y = (b_eXtent - a_eXtent);
		diffrence.Y = (B->pos->Y + B->offset.Y) - (A->pos->Y + A->offset.Y);
		// Calculate overlap on Y aXis
		float Y_overlap = (A->size.Y / 2 + (B->size.Y / 2)) - abs(diffrence.Y);
		//	std::cout << "Y-overlap" << Y_overlap << std::endl;

		// SAT test on Y aXis
		if (Y_overlap >= 0)
		{


			// Find out which aXis is aXis of least penetration
			if (X_overlap < Y_overlap)
			{

				// so collisoion eXist 
				if (A->trigger == 1 || B->trigger == 1)
				{
					// std::cout << "Trigger collision " << std::endl;
					A->detected = true;
					B->detected = true;
					return true;

				}
				if (A->wall || B->wall)
				{

					return 0;
				}
				// Point towards B knowing that n points from A to B
				if (diffrence.X<0)
					// left A - RIgth B
				{
					A->normal = vec2f(-1, 0);
					B->normal = vec2f(1, 0);
					A->left = 1;

					//A->GlobalTransform.Position.X += X_overlap;

					A->pos->X += X_overlap;

				}
				else

				{
					//rigth A - LEFT B
					A->normal = vec2f(1, 0);
					B->normal = vec2f(-1, 0);
					A->rigth = 1;

					//	A->GlobalTransform.Position.X -= X_overlap;

					A->pos->X -= X_overlap;
				}

				A->penetration = X_overlap;
				B->penetration = X_overlap;

				A->detected = true;
				B->detected = true;


				return true;

			}

			else
			{
				// Point toward B knowing that n points from A to B
				if (diffrence.Y<0)
				{
					//down A - UP B 

					//	A->GlobalTransform.Position.Y += Y_overlap;

					A->pos->Y += Y_overlap;

					A->normal = vec2f(0, -1);
					B->normal = vec2f(0, 1);
					A->down = 1;

					// std::cout << "Down " << std::endl;

				}
				else
					//up A - DOWN B 
				{
					// so collisoion eXist 
					if (A->trigger == 1 || B->trigger == 1)
					{
						// std::cout << "Trigger collision " << std::endl;
						A->detected = true;
						B->detected = true;
						return true;

					}
					A->normal = vec2f(0, 1);
					B->normal = vec2f(0, -1);
					A->up = 1;

					//A->GlobalTransform.Position.Y -= Y_overlap;

					A->pos->Y -= Y_overlap;

					// std::cout << "Up " << std::endl;

				}
				//*****************importatnt
				A->penetration = Y_overlap;
				B->penetration = Y_overlap;

				A->detected = true;
				B->detected = true;

				return true;
			}
		}
	}

	A->detected = false;
	B->detected = false;
	A->normal = vec2f(0, 0);
	B->normal = vec2f(0, 0);

	return false;

}
