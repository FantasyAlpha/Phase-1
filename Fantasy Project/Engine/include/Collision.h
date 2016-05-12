#include<Mesh.h>
#include <Components.h>

struct CollisionFlags
{

	bool jumpflag = 0;

};

bool SweptAABBvsAABB(Collider *A, Collider *B, float delta);

bool AABBvsAABB(Collider *A, Collider *B);