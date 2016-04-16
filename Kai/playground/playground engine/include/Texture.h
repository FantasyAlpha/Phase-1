#pragma once
#include <GL\glew.h>
#include <iostream>
#include <Types.h>
#include <Utilities.h>

struct Texture
{
	unsigned int TextureHandle;	
};

////
Texture LoadTexture(char *imagePath);
void BindTexture(Texture *texture);
void UnbindTexture();