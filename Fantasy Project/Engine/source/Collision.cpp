#include <Collision.h>

CollisionFlags flags;

bool SweptAABBvsAABB(Collider *A, Collider *B, float delta)
{

	//***************
	//owner = collision->Owner;

	vec2f d0, d1;
	vec2f VAB;
	vec2f u0, u1;
	float t0, t1;
	int count = 0;
	int flag = 0;
	VAB.X = (A->velocity.X - (B->velocity.X));
	VAB.Y = A->velocity.Y - (B->velocity.Y);


	vec2f a_extent, b_extent;
	vec2f amin, amax, bmin, bmax;
	vec2f diffrence;

	a_extent.X = A->pos->X + (A->size.X) / 2.0f;
	b_extent.X = B->pos->X + B->size.X / 2.0f;

	//diffrence.X = (b_extent.X - a_extent.X);
	diffrence.X = B->pos->X - A->pos->X;

	a_extent.Y = A->pos->Y + (A->size.Y) / 2.0f;
	b_extent.Y = B->pos->Y + B->size.Y / 2.0f;

	//diffrence.Y = (b_extent.Y - a_extent.Y);
	diffrence.Y = B->pos->Y - A->pos->Y;


	//d0.X = B->collider.pos.X - (A->collider.pos.X + A->collider.size.X);
	//d1.X = B->collider.pos.X + B->collider.size.X - A->collider.pos.X;

	d0.X = abs(diffrence.X) - (A->size.X / 2.0f + B->size.X / 2.0f);
	d0.Y = abs(diffrence.Y) - (A->size.Y / 2.0f + B->size.Y / 2.0f);

	d1.X = abs(diffrence.X) + (A->size.X / 2.0f + B->size.X / 2.0f);
	d1.Y = abs(diffrence.Y) + (A->size.Y / 2.0f + B->size.Y / 2.0f);

	float x_overlap = ((A->size.X) / 2.0f + (B->size.X / 2.0f)) - abs(diffrence.X);
	float y_overlap = (A->size.Y / 2.0f + (B->size.Y / 2.0f)) - abs(diffrence.Y);

	//std::cout << "x-overlap" << x_overlap << std::endl;
	//std::cout << "y-overlap" << y_overlap << std::endl;

	if (abs(A->velocity.X) > 0 && A->velocity.Y == 0)
	{

		if (y_overlap > 0){

			// compute eariler collsion time in x 
			u0.X = abs(d0.X / abs(VAB.X));
			//// compute latest collsion time in x 
			u1.X = abs(d1.X) / abs(VAB.X);
			u0.Y = 0;
			u1.Y = 1.0f;

		}

		else {

			//std::cout << "Noo  collision " << std::endl;
			B->normal = vec2f(0, 0);
			A->normal = vec2f(0, 0);

			A->detected = 0;
			B->detected = 0;
			return false;
		}


	}


	else if (A->velocity.X == 0 && (abs(A->velocity.Y)>0)) {

		if (x_overlap >0)
		{
			u0.Y = abs(d0.Y) / abs(VAB.X);
			u1.Y = abs(d1.Y) / abs(VAB.Y);

			u0.X = 0;
			u1.X = 1.0f;
			count = 2;
		}
		else {
			//	std::cout << "Noo  collision " << std::endl;
			B->normal = vec2f(0, 0);
			A->normal = vec2f(0, 0);
			A->detected = 0;
			B->detected = 0;

			return false;
		}
	}




	if (A->velocity.Y >0 && abs(A->velocity.X) > 0)
	{

		// no checking here for the x , y overlaps cause we move in both directions 
		u0.X = abs(d0.X) / abs(VAB.X);
		u1.X = abs(d1.X) / abs(VAB.X);

		u0.Y = (d0.Y) / (VAB.Y);
		u1.Y = (d1.Y) / (VAB.Y);

		count = 4;
	}

	//Standing away / sticking on one of the sides 

	if (A->velocity.X == 0 && A->velocity.Y == 0)
	{
		//std::cout << "Noo  collision " << std::endl;
		B->normal = vec2f(0, 0);
		A->normal = vec2f(0, 0);
		A->detected = 0;
		B->detected = 0;

		count = 1;

		return false;
	}


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
				A->normal = vec2f(1, 0);
				B->normal = vec2f(-1, 0);
				std::cout << "Rigth " << std::endl;
				//A->GlobalTransform.Position.X -= ((VAB.X)*delta - (d0.X));

				A->pos->X -= ((VAB.X)*.016f - (d0.X));

				A->rigth = 1;
				//	B->left = 1;

			}

			else if (diffrence.X <= 0)
			{
				A->normal = vec2f(-1, 0);
				B->normal = vec2f(1, 0);
				//	std::cout << "left " << std::endl;
				//A->GlobalTransform.Position.X += (abs(VAB.X)*delta - (d0.X));

				A->pos->X += (abs(VAB.X)*.016f - (d0.X));

				A->left = 1;
				//B->rigth = 1;

			}

			//**********************importannnt
			A->velocity.X = 0.0f;
			//	B->velocity.X = 0.0f;

		}

		else if (u0.Y>u0.X || flag == 2) {

			if (diffrence.Y > 0)
			{

				A->normal = vec2f(0, 1);
				B->normal = vec2f(0, -1);
				//	std::cout << "up " << std::endl;

				// up A down B
				//A->GlobalTransform.Position.Y -= ((VAB.Y)*delta - (d0.Y));
				A->pos->Y -= ((VAB.Y)*.016f - (d0.Y));

				// no more jumping 
				A->up = 1;
				//B->down = 1;

			}

			else  if (diffrence.Y <= 0)
			{

				A->normal = vec2f(0, -1);
				B->normal = vec2f(0, 1);
				//	std::cout << "down " << std::endl;

				// down A UP B
				//fall
				//A->GlobalTransform.Position.Y += abs(VAB.Y)*.016f;
				A->pos->Y += abs(VAB.Y)*.016f;

				A->down = 1;
				//  B->up = 1;

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
		A->normal = vec2f(0, 0);
		B->normal = vec2f(0, 0);

		//std::cout << " disjoint  " << std::endl;

		return false;

	}

}
bool AABBvsAABB(Collider *A, Collider *B)
{

	// Calculate half eXtents along X aXis for each object
	float a_eXtent = A->pos->X + (A->size.X) / 2;
	float b_eXtent = B->pos->X + B->size.X / 2;
	vec2f diffrence;
	//diffrence.X = (b_eXtent - a_eXtent);
	diffrence.X = B->pos->X - A->pos->X;
	// Calculate overlap on X aXis
	float X_overlap = ((A->size.X) / 2 + (B->size.X / 2)) - abs(diffrence.X);
	//	std::cout << "X-overlap" <<X_overlap<< std::endl;
	// SAT test on X aXis

	if (X_overlap >= 0)
	{

		// Calculate half eXtents along X aXis for each object
		float a_eXtent = A->pos->Y + (A->size.Y / 2);
		float b_eXtent = B->pos->Y + (B->size.Y / 2);
		//diffrence.Y = (b_eXtent - a_eXtent);
		diffrence.Y = B->pos->Y - A->pos->Y;
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
				/*if (A->wall || B->wall)
				{

				return 0;
				}*/
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
