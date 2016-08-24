#pragma once

#include <unordered_map>
#include <Cinder\CinderMemory.h>
#include "Types.h"
#include "Transform.h"
#include "RenderingEngine.h"
#include "PhysicsEngine.h"
#include "GameData.h"

struct SceneManager
{	
public:
	SceneManager()
	{
		
	}

	~SceneManager(){}
public:
	void Init(uint32 maxObjectCount = 2);

	GameObject* AddObject(char *name, char *tag = "", char *layer = "");
	GameObject* AddObject(GameObject *object);
	
	GameObject* GetGameObject(char *name);
	
	void RemoveObject(char *name);
	
	uint32 GetInputKey(int button);
	
	void Render();
	void Update(float delta, Game_Input *input);

	GameComponent* AddComponent(GameComponent *component);
	void AddComponentPool(GameComponent *component);
	
	Cinder::Memory::MemoryPool* GetComponentPool(char *type);
	std::vector<GameComponent *>* GetAllComponents(char *type);

	GameObject* AddSubRoot(char *name);

	GameObject* GetSubRoot(char *name);

	void RemoveSubRoot(char *name);

	void EnableSubRoot(char *name);

private:
	void ActivateSubRoot(char *name);
	GameObject* CreateObject(char *name, char *tag = "", char *layer = "");

	GameObject* GetGameObject(uint32 index);

	void RemoveChildren(char *name);

	void RemoveComponentPool(char *type);	
	void RemoveComponent(GameComponent *comp);
	void RemoveAllComponents(char *name);
private:
	typedef std::unordered_map<char *, GameObject *> Objects;
	std::unordered_map<char *, Objects *> StateObjects;
	std::unordered_map<char*, GameObject *> SubRoots;
	std::unordered_map<char *, Cinder::Memory::MemoryPool> ComponentPools;
	std::unordered_map<char *, std::vector<GameComponent*>> AllComponents;
	Cinder::Memory::MemoryPool ObjectPool;
	bool Debug;
public:
	PhysicsEngine PEngine;
	RenderingEngine REngine;
	Game_Input *Input;
	BasicCamera MainCamera;
	float Delta;
	GameObject *Root;
	GameObject *CurrentRoot;
};