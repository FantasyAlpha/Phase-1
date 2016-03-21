#include <World.h>

void World::RenderWorld()
{
	this->RendererManager.RenderAllActive();
}

void World::InitWorld(StackAllocator *sourceAllocator)
{	
	this->ActorManager.Owner = this;
	this->RendererManager.Owner = this;
	this->TransformManager.Owner = this;
	this->ActorManager.InitActorSystem(sourceAllocator);
	this->RendererManager.InitRendererSystem(sourceAllocator);
	this->TransformManager.InitTransformSystem(sourceAllocator);
}

void World::UpdateWorld()
{
	this->TransformManager.UpdateTransformSystem();
}