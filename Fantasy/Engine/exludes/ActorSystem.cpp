#if 0

#include "Components.h"

void ActorSystem::InitActorSystem(uint32 count)
{
	Pool = Cinder::Memory::DoubleLinkedPool(sizeof(Actor), count, false);
	this->Actors = (Actor*)Pool.Elements;
	AddActor("MAIN PARENT", Transform{});
}

void ActorSystem::AddActor(char *name, Transform transform, char *tag)
{	
	uint32 index = GetActorIndex(name);
	if (index == Pool.ChunkCount + 1)
	{
		Cinder::Memory::PoolHeader block = Pool.Alloc();
		Actors[block.Current].CurrentTransform = transform;
		Actors[block.Current].OldTransform = transform;
		Actors[block.Current].Children = new std::vector<uint32>();
		Actors[block.Current].ParentName = NULL;
		Actors[block.Current].Name = name;
		Actors[block.Current].Tag = tag;
		Actors[block.Current].ParentIndex = Pool.ChunkCount + 1;
		Actors[block.Current].ModelMatrix = mat4f();
		ActiveActorMap[name] = block.Current;

		if (!MainParent)
		{
			SetMainParent(name);
		}
		else
		{
			AttachChild(Actors[MainParentIndex].Name, name);
		}		
	}	
}

Actor* ActorSystem::GetActor(char *name)
{
	if (ActiveActorMap.find(name) != ActiveActorMap.end())
	{
		return &Actors[this->ActiveActorMap[name]];
	}

	return NULL;
}

uint32 ActorSystem::GetActorIndex(char *name)
{
	if (ActiveActorMap.find(name) != ActiveActorMap.end())
	{
		return this->ActiveActorMap[name];
	}

	return Pool.ChunkCount + 1;
}

void ActorSystem::DestroyActor(char *name)
{
	uint32 index = GetActorIndex(name);
	if (index != Pool.ChunkCount + 1)
	{
		uint32 parentIndex = Actors[index].ParentIndex;
		
		RemoveChildren(name, index);

		Owner->CollisionManager.RemoveComponent(name);
		Owner->RendererManager.RemoveRenderer(name, RenderableType::Static);
		Owner->RendererManager.RemoveRenderer(name, RenderableType::Movable);

		Pool.Dealloc(index);
		if (parentIndex != Pool.ChunkCount + 1)
		{
			for (uint32 i = 0; i < (*Actors[parentIndex].Children).size(); i++)
			{
				if ((*Actors[parentIndex].Children)[i] == index)
				{
					(*Actors[parentIndex].Children).erase((*Actors[parentIndex].Children).begin() + i);
					break;
				}
			}
			//(*Actors[parentIndex].Children).erase((*Actors[parentIndex].Children).begin() + index);
		}

		ActiveActorMap.erase(name);
	}
}

void ActorSystem::RemoveChildren(char *name, uint32 index)
{
	if ((*Actors[index].Children).size() == 0)
	{
		return;
	}

	for (uint32 i = 0; i < (*Actors[index].Children).size(); i++)
	{
		uint32 childIndex = (*Actors[index].Children)[i];
		RemoveChildren(Actors[childIndex].Name, (*Actors[index].Children)[i]);
		Owner->CollisionManager.RemoveComponent(Actors[childIndex].Name);
		Owner->RendererManager.RemoveRenderer(Actors[childIndex].Name, RenderableType::Static);
		Owner->RendererManager.RemoveRenderer(Actors[childIndex].Name, RenderableType::Movable);


		Pool.Dealloc((*Actors[index].Children)[i]);
		Actors[index].Children->erase(Actors[index].Children->begin() + i);
		i--;
	}
}

void ActorSystem::AttachChild(char *parentName, char *childName)
{
	uint32 childIndex = GetActorIndex(childName);
	uint32 newParentIndex = GetActorIndex(parentName);
	uint32 oldParentIndex = GetActorIndex(Actors[childIndex].ParentName);

	if (parentName != childName
		&& newParentIndex != Pool.ChunkCount + 1
		&& childIndex != Pool.ChunkCount + 1)
	{

		if (oldParentIndex != Pool.ChunkCount + 1)
		{
			uint32 index = Pool.ChunkCount + 1;
			for (uint32 i = 0; i < (*Actors[oldParentIndex].Children).size(); i++)
			{
				if ((*Actors[oldParentIndex].Children)[i] == childIndex)
				{
					index = i;
					break;
				}
			}

			for (uint32 i = 0; i < (*Actors[oldParentIndex].Children).size(); i++)
			{
				if ((*Actors[oldParentIndex].Children)[i] == index)
				{
					(*Actors[oldParentIndex].Children).erase((*Actors[oldParentIndex].Children).begin() + i);
					break;
				}
			}
		}

		if (Actors[newParentIndex].ParentName != childName)
		{
			Actors[childIndex].ParentName = parentName;
			Actors[childIndex].ParentIndex = ActiveActorMap[parentName];
			(*Actors[newParentIndex].Children).push_back(childIndex);
		}
	}
}

void ActorSystem::SetMainParent(char *name)
{
	if (ActiveActorMap.find(name) != ActiveActorMap.end() && Pool.UsedHeaders[ActiveActorMap[name]].Current != Pool.ChunkCount + 1)
	{
		uint32 index = ActiveActorMap[name];
		MainParent = &Actors[index];
		MainParent->ParentIndex = Pool.ChunkCount + 1;
		MainParent->ParentName = NULL;
		MainParentIndex = index;
	}
}

void ActorSystem::UpdateTransforms()
{
	UpdateTransformChildren(MainParent, false);
	UpdateOldTransforms(MainParent);
}

void ActorSystem::UpdateTransformChildren(Actor *transform, bool parentChanged)
{
	if ((*transform->Children).size() == 0)
	{
		if ((!parentChanged || transform->ParentIndex > Pool.ChunkCount)
			&& (transform->OldTransform.Position != transform->CurrentTransform.Position
			|| transform->OldTransform.Rotation != transform->CurrentTransform.Rotation
			|| transform->OldTransform.Scale != transform->CurrentTransform.Scale))
		{
			transform->ModelMatrix = CalcModelMatrix(&transform->CurrentTransform);
			if (transform->ParentIndex < Pool.ChunkCount)
			{
				transform->ModelMatrix = Actors[transform->ParentIndex].ModelMatrix * transform->ModelMatrix;
			}
		}
		else if (parentChanged)
		{
			transform->ModelMatrix = Actors[transform->ParentIndex].ModelMatrix * CalcModelMatrix(&transform->CurrentTransform);
		}
	}

	for (uint32 i = 0; i < transform->Children->size(); i++)
	{
		bool didChange = false;

		if ((!parentChanged || transform->ParentIndex > Pool.ChunkCount)
			&& (transform->OldTransform.Position != transform->CurrentTransform.Position
			|| transform->OldTransform.Rotation != transform->CurrentTransform.Rotation
			|| transform->OldTransform.Scale != transform->CurrentTransform.Scale))
		{
			if (i == 0)
			{
				transform->ModelMatrix = CalcModelMatrix(&transform->CurrentTransform);
				if (transform->ParentIndex < Pool.ChunkCount)
				{
					transform->ModelMatrix = Actors[transform->ParentIndex].ModelMatrix * transform->ModelMatrix;
				}
			}

			didChange = true;
		}
		else if (parentChanged)
		{
			if (i == 0)
			{
				transform->ModelMatrix = Actors[transform->ParentIndex].ModelMatrix * CalcModelMatrix(&transform->CurrentTransform);
			}

			didChange = true;
		}

		UpdateTransformChildren(&Actors[(*transform->Children)[i]], didChange);
	}
}

void ActorSystem::UpdateOldTransforms(Actor *transform)
{
	for (uint32 i = 0; i < transform->Children->size(); i++)
	{
		if (i == 0)
		{
			transform->OldTransform = transform->CurrentTransform;
		}

		UpdateOldTransforms(&Actors[(*transform->Children)[i]]);
	}

	if (transform->Children->size() == 0)
	{
		transform->OldTransform = transform->CurrentTransform;
	}
}


Transform* ActorSystem::GetTransform(char *name)
{
	uint32 index = GetActorIndex(name);

	if (index < Pool.ChunkCount)
	{
		return &Actors[index].CurrentTransform;
	}

	return NULL;
}

#endif