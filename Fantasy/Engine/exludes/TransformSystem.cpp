//#include <Components.h>
//#include <World.h>
//
//void TransformSystem::InitTransformSystem()
//{
//	Pool = Cinder::Memory::DoubleLinkedPool(sizeof(TransformComponent), MAX_TRANSFORM_COUNT, false);
//	Transforms = (TransformComponent*)Pool.Elements;
//}
//
//void TransformSystem::AddComponent(char *actorName, TransformComponent transform)
//{
//	uint32 actorIndex = Owner->ActorManager.GetActorIndex(actorName);
//	if (actorIndex < MAX_ACTOR_COUNT)
//	{
//		if (Pool.UsedHeaders[actorIndex].Current == Pool.ChunkCount + 1)
//		{
//			Cinder::Memory::PoolHeader header = Pool.Alloc();
//
//			Transforms[header.Current] = transform;
//			Transforms[header.Current].Children = new std::vector<uint32>();
//			Transforms[header.Current].ParentIndex = MAX_TRANSFORM_COUNT + 1;
//			if (!MainParent)
//			{
//				SetMainParent(actorName);
//			}
//			else
//			{
//				AttachTransformChild(GetTransformOwner(MainParentIndex), actorName);
//			}
//		}
//	}
//}
//
//uint32 TransformSystem::GetTransformIndex(char *name)
//{
//	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);
//	if (Owner->ActorManager.Pool.UsedHeaders[actorIndex].Current != Owner->ActorManager.Pool.ChunkCount + 1)
//	{
//		return Owner->ActorManager.GetActorIndex(name);
//	}
//
//	return MAX_TRANSFORM_COUNT + 1;
//}
//
//char* TransformSystem::GetTransformOwner(uint32 index)
//{
//	return Owner->ActorManager.Actors[index].Name;
//}
//
//void TransformSystem::RemoveTransformChildren(char *name, uint32 index)
//{
//	if ((*Transforms[index].Children).size() == 0)
//	{
//		return;
//	}
//
//	for (uint32 i = 0; i < (*Transforms[index].Children).size(); i++)
//	{
//		RemoveTransformChildren(GetTransformOwner((*Transforms[index].Children)[i]), (*Transforms[index].Children)[i]);
//		Pool.Dealloc((*Transforms[index].Children)[i]);
//		Transforms[index].Children->erase(Transforms[index].Children->begin() + i);
//		i--;
//	}
//}
//
//void TransformSystem::RemoveTransform(char *name)
//{
//	if (Pool.UsedHeaders[Owner->ActorManager.GetActorIndex(name)].Current != Pool.ChunkCount + 1)
//	{
//		uint32 index = GetTransformIndex(name);
//		uint32 parentIndex = GetTransformIndex(Transforms[index].ParentName);
//		RemoveTransformChildren(name, index);
//		Pool.Dealloc(index);
//		if (parentIndex < MAX_TRANSFORM_COUNT)
//		{
//			(*Transforms[parentIndex].Children).erase((*Transforms[parentIndex].Children).begin() + index);
//		}
//	}
//}
//
//void TransformSystem::AttachTransformChild(char *parentActorName, char *childActorName)
//{
//	if (parentActorName != childActorName 
//		&& Owner->ActorManager.ActiveActorMap.find(parentActorName) != Owner->ActorManager.ActiveActorMap.end()
//		&& Owner->ActorManager.ActiveActorMap.find(childActorName) != Owner->ActorManager.ActiveActorMap.end()
//		&& Pool.UsedHeaders[Owner->ActorManager.ActiveActorMap[parentActorName]].Current != Pool.ChunkCount + 1
//		&& Pool.UsedHeaders[Owner->ActorManager.ActiveActorMap[childActorName]].Current != Pool.ChunkCount + 1)
//	{
//		uint32 childIndex = GetTransformIndex(childActorName);
//		uint32 oldParentIndex = GetTransformIndex(Transforms[childIndex].ParentName);
//		uint32 newParentIndex = GetTransformIndex(parentActorName);
//
//		if (oldParentIndex < MAX_TRANSFORM_COUNT)
//		{
//			uint32 index = MAX_TRANSFORM_COUNT + 1;
//			for (uint32 i = 0; i < (*Transforms[oldParentIndex].Children).size(); i++)
//			{
//				if ((*Transforms[oldParentIndex].Children)[i] == childIndex)
//				{
//					index = i;
//					break;
//				}
//			}
//
//			(*Transforms[oldParentIndex].Children).erase((*Transforms[oldParentIndex].Children).begin() + index);
//		}
//
//		if (Transforms[newParentIndex].ParentName != childActorName)
//		{
//			Transforms[childIndex].ParentName = parentActorName;
//			Transforms[childIndex].ParentIndex = Owner->ActorManager.ActiveActorMap[parentActorName];
//			(*Transforms[newParentIndex].Children).push_back(childIndex);
//		}
//	}	
//}
//
//void TransformSystem::SetMainParent(char *name)
//{
//	if (Owner->ActorManager.ActiveActorMap.find(name) != Owner->ActorManager.ActiveActorMap.end() && Pool.UsedHeaders[Owner->ActorManager.ActiveActorMap[name]].Current != Pool.ChunkCount + 1)
//	{
//		if (!MainParent)
//		{
//			MainParent = &Transforms[Owner->ActorManager.ActiveActorMap[name]];
//			MainParentIndex = GetTransformIndex(name);
//		}
//	}
//}
//
//void TransformSystem::UpdateTransformSystem()
//{
//	UpdateTransformChildren(MainParent, false);
//	UpdateOldTransform(MainParent);
//}
//
//void TransformSystem::UpdateTransformChildren(TransformComponent *transform, bool parentChanged)
//{
//	if ((*transform->Children).size() == 0)
//	{
//		if ((!parentChanged || transform->ParentIndex > MAX_TRANSFORM_COUNT)
//			&& (transform->OldTransform.Position != transform->CurrentTransform.Position
//			|| transform->OldTransform.Rotation != transform->CurrentTransform.Rotation
//			|| transform->OldTransform.Scale != transform->CurrentTransform.Scale))
//		{
//			transform->ModelMatrix = CalcModelMatrix(&transform->CurrentTransform);
//			if (transform->ParentIndex < MAX_TRANSFORM_COUNT)
//			{
//				transform->ModelMatrix = Transforms[transform->ParentIndex].ModelMatrix * transform->ModelMatrix;
//			}
//		}
//		else if (parentChanged)
//		{
//			transform->ModelMatrix = Transforms[transform->ParentIndex].ModelMatrix * CalcModelMatrix(&transform->CurrentTransform);
//		}
//	}
//
//	for (uint32 i = 0; i < transform->Children->size(); i++)
//	{
//		bool didChange = false;		
//
//		if ((!parentChanged || transform->ParentIndex > MAX_TRANSFORM_COUNT)
//			&& (transform->OldTransform.Position != transform->CurrentTransform.Position
//			|| transform->OldTransform.Rotation != transform->CurrentTransform.Rotation
//			|| transform->OldTransform.Scale != transform->CurrentTransform.Scale))
//		{
//			if (i == 0)
//			{
//				transform->ModelMatrix = CalcModelMatrix(&transform->CurrentTransform);
//				if (transform->ParentIndex < MAX_TRANSFORM_COUNT)
//				{
//					transform->ModelMatrix = Transforms[transform->ParentIndex].ModelMatrix * transform->ModelMatrix;
//				}
//			}
//
//			didChange = true;
//		}
//		else if (parentChanged)
//		{
//			if (i == 0)
//			{
//				transform->ModelMatrix = Transforms[transform->ParentIndex].ModelMatrix * CalcModelMatrix(&transform->CurrentTransform);
//			}
//
//			didChange = true;
//		}
//
//		UpdateTransformChildren(&Transforms[(*transform->Children)[i]], didChange);
//	}
//}
//
//void TransformSystem::UpdateOldTransform(TransformComponent *transform)
//{
//	for (uint32 i = 0; i < transform->Children->size(); i++)
//	{
//		if (i == 0)
//		{
//			transform->OldTransform = transform->CurrentTransform;
//		}
//
//		UpdateOldTransform(&Transforms[(*transform->Children)[i]]);
//	}
//
//	if (transform->Children->size() == 0)
//	{
//		transform->OldTransform = transform->CurrentTransform;
//	}
//}
//
//
//Transform* TransformSystem::GetTransform(char *name)
//{
//	uint32 index = GetTransformIndex(name);
//
//	if (index < MAX_TRANSFORM_COUNT)
//	{
//		return &Transforms[index].CurrentTransform;
//	}
//
//	return NULL;
//}
//
