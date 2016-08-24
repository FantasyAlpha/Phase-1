#pragma once

#include <GL\glew.h>
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameData.h"

struct BasicGame
{
public:
	BasicGame(){}
	virtual ~BasicGame(){}
public:
	virtual void Init(){}

	void InitSystem()
	{		
		Scene = new SceneManager();
		Scene->Input = &Input;
		Dimensions = {};
	}

	void Render()
	{
		Scene->Render();
	}

	void Update(float delta)
	{
		Scene->Update(delta, &Input);
	}	

	void UpdateViewSize(float width, float height)
	{
		Dimensions.Width = width;
		Dimensions.Height = height;
		Scene->MainCamera.Size.X = width * 2.5f;
		Scene->MainCamera.Size.Y = height * 2.5f;
	}

	void ShutDown(){}

public:
	SceneManager *Scene;
	Window_Dimensions Dimensions;
	Game_Resources ResourceManager;
	Game_Input Input;
};

