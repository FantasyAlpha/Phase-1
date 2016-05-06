#include <World.h>

void World::RenderWorld(bool debug)
{
	this->RendererManager.RenderAllActive();

	if (debug)
	{
		this->RendererManager.RenderDebugShapes();
	}
}

void World::InitWorld(StackAllocator *sourceAllocator)
{	
	this->ActorManager.Owner = this;
	this->RendererManager.Owner = this;
	this->TransformManager.Owner = this;
	this->CollisionManager.Owner = this;
    this->AnimationManager.Owner = this;

	this->ActorManager.InitActorSystem(sourceAllocator);
	this->RendererManager.InitRendererSystem(sourceAllocator);
	this->TransformManager.InitTransformSystem(sourceAllocator);
	this->CollisionManager.InitCollisionSystem(sourceAllocator);
	this->AnimationManager.InitAnimationSystem(sourceAllocator);
}

void World::UpdateWorld(float delta)
{
	ActivateShader(&RendererManager.MainShader);
	glUniform2f(GetUniformLocation(&RendererManager.MainShader, UNIFORMS::MOUSE_POS), this->MousePos.x, this->MousePos.y);
	this->CollisionManager.UpdateCollisionSystem(delta);
	this->TransformManager.UpdateTransformSystem();

}