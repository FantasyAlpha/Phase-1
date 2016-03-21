#include <Components.h>
#include <World.h>

void RendererSystem::InitRendererSystem(StackAllocator *sourceAllocator)
{
	MaxSize = TOTAL_SPRITES_SIZE;
	InitPartialPoolSystem(sourceAllocator, &Allocator, MaxSize, SPRITE_RENDERER_SIZE);
	Renderers = (Renderer*)Allocator.Blocks->Dimensions.BaseAddress;
}

void RendererSystem::AddComponent(char *actorName, Renderer renderer)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(actorName);
	if (actorIndex < MAX_ACTOR_COUNT)
	{
		if (!Allocator.Blocks[actorIndex].IsUsed)
		{
			PoolAlloc(&Allocator, actorIndex);

			Renderers[actorIndex] = renderer;
		}
	}	
}

uint32 RendererSystem::GetRendererIndex(char *name)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);
	if (Allocator.Blocks[actorIndex].IsUsed)
	{
		return Owner->ActorManager.GetActorIndex(name);
	}

	return MAX_ACTOR_COUNT + 1;
}

void RendererSystem::RemoveRenderer(char *actorName)
{
	if (GetRendererIndex(actorName) < MAX_ACTOR_COUNT)
	{
		uint32 actorIndex = Owner->ActorManager.GetActorIndex(actorName);
		PoolDealloc(&Allocator, actorIndex);
	}
}

void RendererSystem::RenderAllActive()
{
	ActivateShader(&MainShader);
	
	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::PROJECTION_MATRIX), 1, false, OrthoProjectionMatrix(&Owner->MainCamera, 1280, 720, -0.1f, 500.0f).GetElemets());
	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::VIEW_MATRIX), 1, false, LookAtViewMatrix(&Owner->MainCamera, vec3(0, 0, -1), vec3(0, 1, 0)).GetElemets());

	uint32 index = Allocator.FirstUsed;

	while (index != Allocator.BlockCount + 1)
	{
		glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::MODEL_MATRIX), 1, false, Owner->TransformManager.Transforms[index].ModelMatrix.GetElemets());
		BindTexture(&Renderers[index].RenderableMaterial.MeshTexture);
		DrawMesh(&Renderers[index].Renderable);
		UnbindTexture();
		index = Allocator.Blocks[index].NextUsedIndex;
	}
}

void RendererSystem::InitMainShader(char *vertexShader, char *fragmentShader)
{
	MainShader = CreateShader(vertexShader, fragmentShader);
}