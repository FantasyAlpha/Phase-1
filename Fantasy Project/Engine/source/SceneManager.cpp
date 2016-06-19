#include <Components.h>
#include <SDL2\SDL.h>
#include <SDL_mixer.h>

void SceneManager::RenderScene(bool debug)
{
	this->RendererManager.RenderAllActive();

	if (debug)
	{
		this->RendererManager.RenderDebugShapes();
	}
}

void SceneManager::InitScene(uint32 count)
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

void SceneManager::UpdateScene(float delta)
{
	this->RendererManager.UpdateRenderables();
	this->CollisionManager.UpdateCollisionSystem(delta);
	this->ActorManager.UpdateTransforms();
}