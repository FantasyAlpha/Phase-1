#include "SceneManager.h"
#include "GameComponent.h"

void SceneManager::Init(uint32 maxObjectCount)
{
	ObjectPool = Cinder::Memory::MemoryPool(sizeof(GameObject), maxObjectCount, true);
	Root = NULL;
	CurrentRoot = NULL;
	StateObjects["Main Root"] = new Objects();
	Root = AddObject("Main Root", "Main Root");
	Root->CurrentRoot = "";
	REngine.Init();
	REngine.Scene = this;
	PEngine.Init();
	PEngine.Scene = this;
	Debug = false;
}

GameObject* SceneManager::CreateObject(char *name, char *tag, char *layer)
{
	GameObject *object = GetGameObject(name);
	
	if (!object)
	{
		object = (GameObject *)ObjectPool.Alloc();
		GameObject obj(this, name, tag, layer);
		
		memcpy(object, &obj, sizeof(GameObject));

		if (tag == "SUBROOT")
		{
			object->CurrentRoot = "Main Root";

			Objects *objects = StateObjects[Root->Name];

			(*objects)[name] = object;

			object->Parent = Root;
			Root->AddChild(object);
		}
		else if (CurrentRoot)
		{
			object->CurrentRoot = CurrentRoot->Name;

			Objects *objects = StateObjects[CurrentRoot->Name];

			(*objects)[name] = object;

			object->Parent = CurrentRoot;
			CurrentRoot->AddChild(object);
		}
		else if (Root)
		{
			object->CurrentRoot = "Main Root";

			Objects *objects = StateObjects[Root->Name];

			(*objects)[name] = object;
			
			object->Parent = Root;
			Root->AddChild(object);
		}
	}

	return object;
}

GameObject* SceneManager::AddObject(char *name, char *tag, char *layer)
{
	if (!GetGameObject(name))
	{
		return CreateObject(name, tag, layer);
	}

	return GetGameObject(name);
}

GameObject* SceneManager::AddObject(GameObject *object)
{
	if (!GetGameObject(object->Name))
	{
		CreateObject(object->Name, object->Tag, object->Layer);
	}

	return object;
}

GameObject* SceneManager::GetGameObject(char *name)
{
	if (CurrentRoot)
	{
		if ((*(StateObjects[CurrentRoot->Name])).find(name) != (*(StateObjects[CurrentRoot->Name])).end())
		{
			return (*(StateObjects[CurrentRoot->Name]))[name];
		}
		else if ((*(StateObjects[Root->Name])).find(name) != (*(StateObjects[Root->Name])).end())
		{
			return (*(StateObjects[Root->Name]))[name];
		}
	}
	else if (Root)
	{
		if ((*(StateObjects[Root->Name])).find(name) != (*(StateObjects[Root->Name])).end())
		{
			return (*(StateObjects[Root->Name]))[name];
		}
	}

	return NULL;
}

void SceneManager::RemoveAllComponents(char *name)
{
	GameObject *object = GetGameObject(name);

	if (object)
	{
		for (uint32 i = 0; i < object->Components->size(); i++)
		{
			Cinder::Memory::MemoryPool *pool = GetComponentPool((*object->Components)[i]->Type);
			(*object->Components)[i]->Destroy();
			pool->Free((*object->Components)[i]);

			RemoveComponent((*object->Components)[i]);

			if (pool->UsedCount == 0)
			{
				RemoveComponentPool((*object->Components)[i]->Type);
			}
		}

		object->Components->clear();
	}
}

void SceneManager::RemoveChildren(char *name)
{
	GameObject *object = GetGameObject(name);

	if (object)
	{
		if (object->Children->size() == 0)
		{
			RemoveAllComponents(object->Name);
		}

		for (uint32 i = 0; i < object->Children->size(); i++)
		{
			RemoveAllComponents((*object->Children)[i]->Name);
			RemoveChildren((*object->Children)[i]->Name);
		}

		object->Children->clear();
		delete object->Children;
		delete object->Components;

		ObjectPool.Free(object);
		if (CurrentRoot)
		{
			(*(StateObjects[CurrentRoot->Name])).erase(object->Name);
		}
		else if (Root)
		{
			(*(StateObjects[Root->Name])).erase(object->Name);
		}
	}
}

void SceneManager::RemoveObject(char *name)
{
	if (name != "Main Root")
	{
		GameObject *object = GetGameObject(name);

		if (object)
		{
			RemoveAllComponents(name);
			RemoveChildren(name);
			int32 index = -1;
			for (uint32 i = 0; i < object->Parent->Children->size(); i++)
			{
				if ((*object->Parent->Children)[i]->Name == name)
				{
					index = i;
					break;
				}
			}

			if (index != -1)
			{
				object->Parent->Children->erase(object->Parent->Children->begin() + index);
			}
		}
	}	
}

void SceneManager::Render()
{
	if (CurrentRoot)
	{
		CurrentRoot->RenderAll();
	}
	else if (Root)
	{
		Root->RenderAll();
	}

	REngine.Render();
	
	if (Debug)
	{
		REngine.DebugRender();
	}
}

void SceneManager::Update(float delta, Game_Input *input)
{
	if (input->P.KeyDown)
	{
		Debug = true;
	}

	if (input->O.KeyDown)
	{
		Debug = false;
	}

	Delta = delta;
	
	PEngine.Update();

	if (CurrentRoot)
	{
		CurrentRoot->UpdateAll(delta, input);
	}
	else if (Root)
	{
		Root->UpdateAll(delta, input);
	}
}

void SceneManager::AddComponentPool(GameComponent *component)
{
	if (ComponentPools.find(component->Type) == ComponentPools.end())
	{
		Cinder::Memory::MemoryPool pool;
		pool = Cinder::Memory::MemoryPool(component->Size, 1, true);
		ComponentPools[component->Type] = pool;
		AllComponents[component->Type] = std::vector<GameComponent *>();
	}
}

void SceneManager::RemoveComponentPool(char *type)
{
	Cinder::Memory::MemoryPool *pool = GetComponentPool(type);

	if (pool)
	{
		pool->Destroy();

		ComponentPools.erase(type);
		AllComponents.erase(type);
	}
}

Cinder::Memory::MemoryPool* SceneManager::GetComponentPool(char *type)
{
	if (ComponentPools.find(type) != ComponentPools.end())
	{
		return &ComponentPools[type];
	}

	return NULL;
}

GameComponent* SceneManager::AddComponent(GameComponent *component)
{
	Cinder::Memory::MemoryPool *pool = GetComponentPool(component->Type);
	std::vector<GameComponent *> *components = GetAllComponents(component->Type);

	if (pool)
	{
		GameComponent *comp = (GameComponent *)pool->Alloc();

		memcpy(comp, component, component->Size);

		if (components)
		{
			AllComponents[comp->Type].push_back(comp);
			return comp;
		}
		else
		{
			AllComponents[comp->Type] = std::vector<GameComponent *>();

			components = GetAllComponents(component->Type);

			components->push_back(comp);

			return comp;
		}
	}

	return NULL;
}

std::vector<GameComponent *>* SceneManager::GetAllComponents(char *type)
{
	if (AllComponents.find(type) != AllComponents.end())
	{
		return &AllComponents[type];
	}

	return NULL;
}

void SceneManager::RemoveComponent(GameComponent *comp)
{
	if (comp)
	{
		std::vector<GameComponent *> *components = GetAllComponents(comp->Type);

		if (components)
		{
			int32 index = -1;
			for (uint32 i = 0; i < components->size(); i++)
			{
				if ((*components)[i] == comp)
				{
					index = i;
					break;
				}
			}

			if (index != -1)
			{
				components->erase(components->begin() + index);
			}
		}
	}
}

uint32 SceneManager::GetInputKey(int button)
{
	for (uint32 i = 0; i < BUTTON_COUNT; i++)
	{
		if (Input->Buttons[i].Button == button)
		{
			return i;
		}
	}

	return -1;
}

GameObject* SceneManager::AddSubRoot(char *name)
{
	if (SubRoots.find(name) == SubRoots.end())
	{
		StateObjects[name] = new Objects();

		SubRoots[name] = CreateObject(name, "SUBROOT");

		if (!CurrentRoot)
		{
			CurrentRoot = SubRoots[name];
		}

		return SubRoots[name];
	}

	return SubRoots[name];
}

GameObject* SceneManager::GetSubRoot(char *name)
{
	if (SubRoots.find(name) != SubRoots.end())
	{
		return SubRoots[name];
	}

	return NULL;
}

void SceneManager::RemoveSubRoot(char *name)
{
	if (GetSubRoot(name))
	{
		RemoveObject(name);

		SubRoots.erase(name);
	
		delete StateObjects[name];

		StateObjects.erase(name);
	}
}

void SceneManager::ActivateSubRoot(char *name)
{
	GameObject *subRoot = GetSubRoot(name);

	if (subRoot)
	{
		CurrentRoot = subRoot;
	}
}

void SceneManager::EnableSubRoot(char *name)
{
	std::unordered_map<char *, GameObject *>::iterator it;

	for (it = SubRoots.begin(); it != SubRoots.end(); it++)
	{
		if (it->first != name)
		{
			it->second->Enabled = false;
		}

		if (it->first == name)
		{
			ActivateSubRoot(name);
		}
	}
}