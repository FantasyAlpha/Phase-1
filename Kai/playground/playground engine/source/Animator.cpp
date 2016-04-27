#include <Components.h>
#include <World.h>

void AnimationSystem::InitAnimationSystem(StackAllocator *sourceAllocator)
{
	MaxSize = TOTAL_CLIPS_SIZE;
	InitPartialPoolSystem(sourceAllocator, &Allocator, MaxSize, CLIP_SIZE);
	Clips = (AnimationClip*)Allocator.Blocks->Dimensions.BaseAddress;
}

void AnimationSystem::AddComponent(char *name, uint32 maxCountHorizontal, uint32 maxCountVertical, uint32 *indices
	, uint32 frameCount, float runSpeed_FPS, bool loop)
{
	if (GetAnimationClipIndex(name) > MAX_CLIP_COUNT)
	{
		PoolBlock block = PoolAlloc(&Allocator);

		Clips[block.BlockIndex].MaxCountHorizontal = maxCountHorizontal;
		Clips[block.BlockIndex].MaxCountVertical = maxCountVertical;
		Clips[block.BlockIndex].Indices = new uint32[frameCount];

		for (uint32 i = 0; i < frameCount; i++)
		{
			Clips[block.BlockIndex].Indices[i] = indices[i];
		}

		Clips[block.BlockIndex].FrameCount = frameCount;
		Clips[block.BlockIndex].RunSpeed_FPS = runSpeed_FPS;
		Clips[block.BlockIndex].Delta = Owner->Delta;
		Clips[block.BlockIndex].Loop = loop;
		Clips[block.BlockIndex].TimeElapsed = 0;
		Clips[block.BlockIndex].Counter = 0;

		Clips[block.BlockIndex].FrameWidth = 1.0f / (float)Clips[block.BlockIndex].MaxCountHorizontal;
		Clips[block.BlockIndex].FrameHeight = 1.0f / (float)Clips[block.BlockIndex].MaxCountVertical;

		Clips[block.BlockIndex].Name = name;

		Name_ComponentMap[name] = block.BlockIndex;
	}
}

uint32 AnimationSystem::GetAnimationClipIndex(char *name)
{
	
	if (Name_ComponentMap.find(name) != Name_ComponentMap.end())
	{
		return Name_ComponentMap[name];
	}

	return MAX_CLIP_COUNT + 1;
}

AnimationClip* AnimationSystem::GetAnimationClip(char *name)
{
	uint32 index = GetAnimationClipIndex(name);

	if (index < MAX_CLIP_COUNT)
	{
		return &Clips[index];
	}

	return NULL;
}

void AnimationSystem::RemoveAnimationClip(char *name)
{
	uint32 index = GetAnimationClipIndex(name);
	if (index < MAX_CLIP_COUNT)
	{
		Name_ComponentMap.erase(name);
		PoolDealloc(&Allocator, index);
	}
}

void AnimationSystem::SwitchAnimation(char *actor, char *animation)
{
	Owner->RendererManager.GetRenderable(actor)->Clip = GetAnimationClip(animation);
}