#pragma once 

#include <Cinder\CinderMemory.h>
#include "GameComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

struct RenderingEngine
{
public:
	RenderingEngine()
	{	
	}

	~RenderingEngine(){}
public:
	void Init();
	
	void Render();
	void Update();

	void DebugRender();
	void DebugUpdate();

	void InitShader(char *vertexShader, char *fragmentShader);
	void InitDebugShader(char *vertexShader, char *fragmentShader);

public:
	Shader MainShader = {};
	Shader DebugShader = {};

	MeshBatch DebugBatch = {};

	SceneManager *Scene = 0;
};

