#pragma once

#include <unordered_map>
#include "Transform.h"
#include "GameData.h"
#include <Cinder\CinderMemory.h>

struct GameComponent;
struct SceneManager;

struct GameObject
{
public:
	GameObject()
	{
		this->Enabled = true;
		this->Name = NULL;
		this->Tag = NULL;
		this->Layer = NULL;
		this->Scene = NULL;
		this->ObjectTransform = Transform();
		this->Children = new std::vector<GameObject *>();
		this->Components = new std::vector<GameComponent *>();
	}
	~GameObject(){}
	GameObject(SceneManager *scene, char *name, char *tag = "", char *layer = "");
	
public:
	void RenderAll();
	void UpdateAll(float delta, Game_Input *input);

	GameObject* AddChild(GameObject *object);
	GameObject* AddChild(char *name, char *tag = "", char *layer = "");
	void AddComponent(GameComponent *component);
	GameComponent* GetComponent(char *name, char *type);
	std::vector<GameComponent *>* GetComponent(char *type);
	GameObject* GetChild(char *name);

public:
	SceneManager *Scene;
	GameObject *Parent;
	Transform ObjectTransform;
	std::vector<GameComponent *> *Components;
	std::vector<GameObject *> *Children;
	
	char *Name;
	char *Tag;
	char *Layer;

	char *CurrentRoot;

	bool Enabled;
private:
	uint32 Index;
};
