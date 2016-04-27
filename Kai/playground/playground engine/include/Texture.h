#pragma once
#include <GL\glew.h>
#include <iostream>
#include <Types.h>
#include <Utilities.h>
#include <Lights.h>

struct Texture
{
	unsigned int TextureHandle;	
	unsigned long Width;
	unsigned long Height;
};

struct Material
{
	Texture MeshTexture;
	Color MeshColor;
	//AmbientLight BaseLight;
};

////
Texture LoadTexture(char *imagePath);
void BindTexture(Texture *texture);
void UnbindTexture();