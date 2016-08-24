#include "GameObject.h"
#include "GameComponent.h"
#include "SceneManager.h"

GameObject::GameObject(SceneManager *scene, char *name, char *tag, char *layer)
{
	Name = name;
	Tag = tag;
	Layer = layer;

	Scene = scene;

	ObjectTransform = Transform();

	this->Children = new std::vector<GameObject *>();
	this->Components = new std::vector<GameComponent *>();
}

void GameObject::RenderAll()
{ 
	for (uint32 i = 0; i < Components->size(); i++)
	{
		if ((*Components)[i]->Type != "RENDERABLE")
		{
			(*Components)[i]->Render();
		}
	}

	for (uint32 i = 0; i < Children->size(); i++)
	{
		if ((*Children)[i]->Enabled)
		{
			(*Children)[i]->RenderAll();
		}
	}
}

void GameObject::UpdateAll(float delta, Game_Input *input)
{
	ObjectTransform.Update();

	for (uint32 i = 0; i < Components->size(); i++)
	{
		(*Components)[i]->Update(delta, input);
	}

	for (uint32 i = 0; i < Children->size(); i++)
	{
		if ((*Children)[i]->Enabled)
		{
			(*Children)[i]->UpdateAll(delta, input);
		}
	}
}

GameObject* GameObject::AddChild(GameObject *object)
{
	GameObject *obj = Scene->GetGameObject(object->Name);
	if (obj)
	{
		if (obj->Parent)
		{
			int32 index = -1;
			for (uint32 i = 0; i < obj->Parent->Children->size(); i++)
			{
				if ((*obj->Parent->Children)[i]->Name == obj->Name)
				{
					index = i;
					break;
				}
			}
			if (index != -1)
			{
				obj->Parent->Children->erase(obj->Parent->Children->begin() + index);
			}
		}

		obj->Parent = this;
		obj->ObjectTransform.Parent = &this->ObjectTransform;
		Children->push_back(obj);
	}
	else
	{
		Scene->AddObject(object);

		int32 index = -1;
		for (uint32 i = 0; i < Scene->Root->Children->size(); i++)
		{
			if ((*Scene->Root->Children)[i]->Name == object->Name)
			{
				index = i;
				break;
			}
		}
		if (index != -1)
		{
			Scene->Root->Children->erase(Scene->Root->Children->begin() + index);
		}
		Scene->Root->Children->erase(Scene->Root->Children->begin() + index);
		obj = Scene->GetGameObject(object->Name);
		obj->Parent = this;
		obj->ObjectTransform.Parent = &this->ObjectTransform;
		Children->push_back(obj);
	}

	return obj;
}

GameObject* GameObject::AddChild(char *name, char *tag, char *layer)
{
	GameObject *obj = Scene->GetGameObject(name);
	if (obj)
	{
		if (obj->Parent)
		{
			int32 index = -1;
			for (uint32 i = 0; i < obj->Parent->Children->size(); i++)
			{
				if ((*obj->Parent->Children)[i]->Name == obj->Name)
				{
					index = i;
					break;
				}
			}
			if (index != -1)
			{
				obj->Parent->Children->erase(obj->Parent->Children->begin() + index);
			}
		}

		obj->Parent = this;
		obj->ObjectTransform.Parent = &this->ObjectTransform;
		Children->push_back(obj);
	}
	else
	{
		Scene->AddObject(name, tag, layer);

		int32 index = -1;
		for (uint32 i = 0; i < Scene->Root->Children->size(); i++)
		{
			if ((*Scene->Root->Children)[i]->Name == name)
			{
				index = i;
				break;
			}
		}
		if (index != -1)
		{
			Scene->Root->Children->erase(Scene->Root->Children->begin() + index);
		}
		Scene->Root->Children->erase(Scene->Root->Children->begin() + index);
		obj = Scene->GetGameObject(name);
		obj->Parent = this;
		obj->ObjectTransform.Parent = &this->ObjectTransform;
		Children->push_back(obj);
	}

	return obj;
}

void GameObject::AddComponent(GameComponent *component)
{
	//Cinder::Memory::DoubleLinkedPool *pool = Scene->GetComponentPool(component->Type);
	Cinder::Memory::MemoryPool *pool = Scene->GetComponentPool(component->Type);
	std::vector<GameComponent *> *components = Scene->GetAllComponents(component->Type);

	if (!pool)
	{
		Scene->AddComponentPool(component);
		//pool = Scene->GetComponentPool(component->Type);
	}

	if (!GetComponent(component->Name, component->Type))
	{
		//Scene->AddComponent(component);
		GameComponent *comp = Scene->AddComponent(component);

	//	GameComponent *comp = Scene->GetComponent(component->Type, component->Index);

		comp->Owner = this;
		comp->Scene = Scene;

		comp->PreInit();
		comp->Init();

		Components->push_back(comp);
	}
}

GameComponent* GameObject::GetComponent(char *name, char *type)
{
	for (uint32 i = 0; i < Components->size(); i++)
	{
		if ((*Components)[i]->Name == name)
		{
			if ((*Components)[i]->Type == type)
			{
				return (*Components)[i];
			}
		}
	}

	return NULL;
}

GameObject* GameObject::GetChild(char *name)
{
	for (uint32 i = 0; i < Children->size(); i++)
	{
		if ((*Children)[i]->Name == name)
		{
			return (*Children)[i];
		}
	}

	return NULL;
}

std::vector<GameComponent *>* GameObject::GetComponent(char *type)
{
	std::vector<GameComponent *> components;

	for (uint32 i = 0; i < Components->size(); i++)
	{
		if ((*Components)[i]->Type == type)
		{
			components.push_back((*Components)[i]);
		}
	}

	return &components;
}