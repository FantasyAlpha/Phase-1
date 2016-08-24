#pragma once

#include "GameComponent.h"
#include "SceneManager.h"
#include "Mesh.h"

struct SpriteRenderer
	: public GameComponent
{
	SpriteRenderer(char *name, Sprite sprite, char *type = "RENDERABLE")
	{
		this->Name = name;
		this->Type = type;
		this->SpriteData = sprite;
		this->Size = sizeof(SpriteRenderer);
	}
	
	virtual void PreInit()
	{
		SpriteData.Owner = Owner;
	}

	virtual void Init()
	{
		if (!SpriteData.Buffers.VBO)
		{
			SpriteData.Buffers = CreateSprite(SpriteData.Pos, SpriteData.Size, SpriteData.Skin.MeshColor, false, false);
		}
	}
	virtual void Destroy(){}
	virtual void Update(float delta, Game_Input *input)
	{
	
	}

	virtual void Render()
	{				
		BindTexture(&SpriteData.Skin.MeshTexture);
		DrawMesh(&SpriteData.Buffers, false);
		UnbindTexture();
	}

	Sprite SpriteData;	
};

