#include <Collision.h>


CollisionFlags flags;

bool SweptAABBvsAABB(Collider *A, Collider *B, float delta)
{

	//***************
	//owner = collision->Owner;

	vec2 d0, d1;
	vec2 VAB;
	vec2 u0, u1;
	float t0, t1;
	int count = 0;

	VAB.x = (A->velocity.x - (B->velocity.x));
	VAB.y = A->velocity.y - (B->velocity.y);


	vec2 a_extent, b_extent;
	vec2 amin, amax, bmin, bmax;
	vec2 diffrence;

	a_extent.x = A->pos->x + (A->size.x) / 2.0f;
	b_extent.x = B->pos->x + B->size.x / 2.0f;

	diffrence.x = (b_extent.x - a_extent.x);

	a_extent.y = A->pos->y + (A->size.y) / 2.0f;
	b_extent.y = B->pos->y + B->size.y / 2.0f;

	diffrence.y = (b_extent.y - a_extent.y);


	//d0.x = B->collider.pos.x - (A->collider.pos.x + A->collider.size.x);
	//d1.x = B->collider.pos.x + B->collider.size.x - A->collider.pos.x;

	d0.x = abs(diffrence.x) - (A->size.x / 2.0f + B->size.x / 2.0f);
	d0.y = abs(diffrence.y) - (A->size.y / 2.0f + B->size.y / 2.0f);

	d1.x = abs(diffrence.x) + (A->size.x / 2.0f + B->size.x / 2.0f);
	d1.y = abs(diffrence.y) + (A->size.y / 2.0f + B->size.y / 2.0f);

	float x_overlap = ((A->size.x) / 2.0f + (B->size.x / 2.0f)) - abs(diffrence.x);
	float y_overlap = (A->size.y / 2.0f + (B->size.y / 2.0f)) - abs(diffrence.y);

	//std::cout << "x-overlap" << x_overlap << std::endl;
	//std::cout << "y-overlap" << y_overlap << std::endl;

	if (abs(A->velocity.x) > 0 && A->velocity.y == 0)
	{

		if (y_overlap > 0){

			// compute eariler collsion time in x 
			u0.x = abs(d0.x / abs(VAB.x));
			//// compute latest collsion time in x 
			u1.x = d1.x / abs(VAB.x);
			u0.y = 0;
			u1.y = 1.0f;

		}

		else {

			//std::cout << "Noo  collision " << std::endl;
			B->normal = vec2(0, 0);
			A->normal = vec2(0, 0);

			A->detected = 0;
			B->detected = 0;
			return false;
		}


	}


	else if (A->velocity.x == 0 && (abs(A->velocity.y)>0)) {

		if (x_overlap >0){
			u0.y = abs(d0.y) / abs(VAB.y);
			u1.y = abs(d1.y) / abs(VAB.y);

			u0.x = 0;
			u1.x = 1.0f;
			count = 2;
		}
		else {
		//	std::cout << "Noo  collision " << std::endl;
			B->normal = vec2(0, 0);
			A->normal = vec2(0, 0);
			A->detected = 0;
			B->detected = 0;

			return false;
		}
	}




	if (A->velocity.y >0 && abs(A->velocity.x) > 0)
	{

		// no checking here for the x , y overlaps cause we move in both directions 
		u0.x = (d0.x) / (VAB.x);
		u1.x = (d1.x) / (VAB.x);

		u0.y = (d0.y) / (VAB.y);
		u1.y = (d1.y) / (VAB.y);

		count = 4;
	}

	//Standing away / sticking on one of the sides 

	if (A->velocity.x == 0 && A->velocity.y == 0)
	{
		//std::cout << "Noo  collision " << std::endl;
		B->normal = vec2(0, 0);
		A->normal = vec2(0, 0);
		A->detected = 0;
		B->detected = 0;

		count = 1;

		return false;
	}


	// **************************end of conditions start the real swept test to detect collision or Not ***********************

	t0 = max(u0.x, u0.y);
	t1 = min(u1.x, u1.y);

	//std::cout << "T0   "<<t0<<"   T1   " <<t1<< std::endl;

	if (t0 <= t1 && t0 >= 0 && t0 <= 1) {

		if (A->trigger == 1 || B->trigger == 1)
		{
			//	std::cout << "Trigger collision " << std::endl;

			A->detected = true;
			B->detected = true;

			return true;

		}
			std::cout << "collision " << std::endl;

		if (u0.x > u0.y)
		{
			// left & rigth

			
			if (diffrence.x > 0)
			{

				// R A - L B 
				A->normal = vec2(1, 0);
				B->normal = vec2(-1, 0);
			//	std::cout << "Rigth " << std::endl;
			//A->GlobalTransform.Position.x -= ((VAB.x)*delta - (d0.x));

				A->pos->x -= ((VAB.x)*.016f - (d0.x));

				A->rigth = 1;
			//	B->left = 1;

			}

			else if (diffrence.x <= 0)
			{
				A->normal = vec2(-1, 0);
				B->normal = vec2(1, 0);
				//	std::cout << "left " << std::endl;
				//A->GlobalTransform.Position.x += (abs(VAB.x)*delta - (d0.x));

				A->pos->x += (abs(VAB.x)*.016f - (d0.x));

				A->left = 1;
			//	B->rigth = 1;

			}

			//**********************importannnt
			A->velocity.x = 0.0f;
			B->velocity.x = 0.0f;

		}

		else {

			if (diffrence.y > 0){

				A->normal = vec2(0, 1);
				B->normal = vec2(0, -1);
				//	std::cout << "up " << std::endl;

				// up A down B
				//A->GlobalTransform.Position.y -= ((VAB.y)*delta - (d0.y));
				A->pos->y -= ((VAB.y)*.016f - (d0.y));

				// no more jumping 
				flags.jumpflag = 0;
				A->up = 1;
			//	B->down = 1;

			}

			else  if (diffrence.y <= 0)
			{

				A->normal = vec2(0, -1);
				B->normal = vec2(0, 1);
				//	std::cout << "down " << std::endl;

				// down A UP B
				//fall
				//A->GlobalTransform.Position.y += abs(VAB.y)*.016f;
				A->pos->y += abs(VAB.y)*.016f;

				A->down = 1;
			//	B->up = 1;

			}

			//**********************importannnt
			A->velocity.y = 0;


		}

		A->detected = true;
		B->detected = true;
		return true;

	}

	else 
	{
		A->detected = false;
		B->detected = false;
		A->normal = vec2(0, 0);
		B->normal = vec2(0, 0);

	   std::cout << " disjoint  " << std::endl;

		return false;

	}

}
