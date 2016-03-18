#include "SpriteRenderingSystem.h"

void InitSpriteRendererSystem(StackAllocator *sourceAllocator, World *manager)
{
	InitPartialPoolSystem(sourceAllocator, &manager->RendererSystem.Data.Allocator, TOTAL_SPRITES_SIZE, SPRITE_RENDERER_SIZE);
	manager->RendererSystem.Renderers = (SpriteRenderer*)manager->RendererSystem.Data.Allocator.Blocks->Dimensions.BaseAddress;
}

void AddRenderer(World *manager, char *actorName, SpriteRenderer renderer)
{
	uint32 actorIndex = GetActorIndex(manager, actorName);
	if (actorIndex < MAX_ACTOR_COUNT)
	{
		if (!manager->RendererSystem.Data.Allocator.Blocks[actorIndex].IsUsed)
		{

			PoolAlloc(&manager->RendererSystem.Data.Allocator, actorIndex);

			manager->RendererSystem.Renderers[actorIndex] = renderer;

			manager->RendererSystem.Data.ActiveComponents.push_back(actorIndex);
		}
	}	
}

uint32 GetRendererIndex(World *manager, char *name)
{
	uint32 actorIndex = GetActorIndex(manager, name);
	if (manager->RendererSystem.Data.Allocator.Blocks[actorIndex].IsUsed)
	{
		return GetActorIndex(manager, name);
	}

	return MAX_ACTOR_COUNT + 1;
}

void RemoveRenderer(World *manager, char *actorName)
{
	if (GetRendererIndex(manager, actorName) < MAX_ACTOR_COUNT)
	{
		uint32 actorIndex = GetActorIndex(manager, actorName);
		PoolDealloc(&manager->RendererSystem.Data.Allocator, actorIndex, false);
		manager->RendererSystem.Data.ActiveComponents.erase(manager->RendererSystem.Data.ActiveComponents.begin() + actorIndex);
	}
}

void RendererSprites(World *manager)
{
	ActivateShader(&manager->RendererSystem.MainShader);
	
	glUniformMatrix4fv(GetUniformLocation(&manager->RendererSystem.MainShader, UNIFORMS::PROJECTION_MATRIX), 1, false, OrthoProjectionMatrix(&manager->Cam, 1280, 720, -0.1f, 500.0f).GetElemets());
	glUniformMatrix4fv(GetUniformLocation(&manager->RendererSystem.MainShader, UNIFORMS::VIEW_MATRIX), 1, false, LookAtViewMatrix(&manager->Cam, vec3(0, 0, -1), vec3(0, 1, 0)).GetElemets());

	for (uint32 i = 0; i < manager->RendererSystem.Data.ActiveComponents.size(); i++)
	{
		glUniformMatrix4fv(GetUniformLocation(&manager->RendererSystem.MainShader, UNIFORMS::MODEL_MATRIX), 1, false, manager->TransformManager.Transforms[manager->RendererSystem.Data.ActiveComponents[i]].ModelMatrix.GetElemets());
		BindTexture(&manager->RendererSystem.Renderers[manager->RendererSystem.Data.ActiveComponents[i]].SpriteMaterial.Tex);
		DrawMesh(&manager->RendererSystem.Renderers[manager->RendererSystem.Data.ActiveComponents[i]].Sprite);
		UnbindTexture();
	}
}

void InitMainShader(World *manager, char *vertexShader, char *fragmentShader)
{
	manager->RendererSystem.MainShader = CreateShader(vertexShader, fragmentShader);
}