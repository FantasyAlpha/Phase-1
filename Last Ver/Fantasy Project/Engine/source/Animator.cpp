#include <Components.h>

void AnimationSystem::InitAnimationSystem(uint32 count)
{
	Pool = Cinder::Memory::MemoryPool(sizeof(AnimationClip), count, false);
	Clips = (AnimationClip*)Pool.Elements;
}

void AnimationSystem::AddComponent(char *name, uint32 maxCountHorizontal, uint32 maxCountVertical, uint32 *indices
	, uint32 frameCount, float runSpeed, bool loop)
{
	if (GetAnimationClipIndex(name) > Pool.ChunkCount)
	{
		Cinder::Memory::PoolHeader block = Pool.Alloc();

		Clips[block.Current].MaxCountHorizontal = maxCountHorizontal;
		Clips[block.Current].MaxCountVertical = maxCountVertical;
		Clips[block.Current].Indices = new uint32[frameCount];

		for (uint32 i = 0; i < frameCount; i++)
		{
			Clips[block.Current].Indices[i] = indices[i];
		}

		Clips[block.Current].FrameCount = frameCount;
		Clips[block.Current].UPS = 1.0f / Owner->Delta;
		Clips[block.Current].RunSpeed_FPS = Owner->Delta * runSpeed;
		Clips[block.Current].Loop = loop;
		Clips[block.Current].TimeElapsed = 0;
		Clips[block.Current].Counter = 0;

		Clips[block.Current].FrameWidth = 1.0f / (float)Clips[block.Current].MaxCountHorizontal;
		Clips[block.Current].FrameHeight = 1.0f / (float)Clips[block.Current].MaxCountVertical;

		Clips[block.Current].Name = name;

		Name_ComponentMap[name] = block.Current;
	}
}

uint32 AnimationSystem::GetAnimationClipIndex(char *name)
{
	
	if (Name_ComponentMap.find(name) != Name_ComponentMap.end())
	{
		return Name_ComponentMap[name];
	}

	return Pool.ChunkCount + 1;
}

AnimationClip* AnimationSystem::GetAnimationClip(char *name)
{
	uint32 index = GetAnimationClipIndex(name);

	if (index < Pool.ChunkCount)
	{
		return &Clips[index];
	}

	return NULL;
}

void AnimationSystem::RemoveAnimationClip(char *name)
{
	uint32 index = GetAnimationClipIndex(name);
	if (index < Pool.ChunkCount)
	{
		Name_ComponentMap.erase(name);
		Pool.Dealloc(index);
	}
}

void AnimationSystem::SwitchAnimation(char *actor, RenderableType type, char *animation, Texture *texture)
{
	if (texture)
	{
		Owner->RendererManager.GetRenderable(actor, type)->Clip = GetAnimationClip(animation);
		Owner->RendererManager.GetRenderable(actor, type)->RenderableMaterial.MeshTexture = *texture;
		if (!Owner->RendererManager.GetRenderable(actor, type)->Clip->Loop)
		{
			Owner->RendererManager.GetRenderable(actor, type)->Clip->Counter = 0;
			Owner->RendererManager.GetRenderable(actor, type)->Clip->TimeElapsed = 0;
		}
	}
	else
	{
		Owner->RendererManager.GetRenderable(actor, type)->Clip = GetAnimationClip(animation);
		if (!Owner->RendererManager.GetRenderable(actor, type)->Clip->Loop)
		{
			Owner->RendererManager.GetRenderable(actor, type)->Clip->Counter = 0;
			Owner->RendererManager.GetRenderable(actor, type)->Clip->TimeElapsed = 0;
		}
	}
}