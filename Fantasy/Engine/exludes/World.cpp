#if 0
#include "Components.h"
#include <SDL2\SDL.h>
#include <SDL_mixer.h>

void World::RenderScene(bool debug)
{
	this->RendererManager.RenderAllActive();

	if (debug)
	{
		this->RendererManager.RenderDebugShapes();
	}
}

void World::InitScene(uint32 count)
{	
	this->ActorManager.Owner = this;
	this->RendererManager.Owner = this;
	this->CollisionManager.Owner = this;
    this->AnimationManager.Owner = this;

	this->ActorManager.InitActorSystem(count);
	this->RendererManager.InitRendererSystem(count);
	this->CollisionManager.InitCollisionSystem(count);
	this->AnimationManager.InitAnimationSystem(count);
}

void World::UpdateScene(float delta)
{
	this->CollisionManager.UpdateCollisionSystem(delta);
	this->ActorManager.UpdateTransforms();
	this->RendererManager.UpdateRenderables();
}

#endif