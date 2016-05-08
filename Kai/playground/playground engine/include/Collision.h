#include<Mesh.h>
#include <Components.h>
#include <World.h>

struct CollisionFlags
{

	bool jumpflag = 0;

};

bool SweptAABBvsAABB(Collider *A, Collider *B, float delta);

bool AABBvsAABB(Mesh *A, Mesh *B);