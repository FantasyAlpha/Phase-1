#pragma once
#include <GL\glew.h>
#include <iostream>
#include "..\Common\Types.h"
#include "..\Utilities\Utilities.h"

struct Texture
{
	unsigned int TextureHandle;	
};

////
Texture LoadTexture(char *imagePath);
void BindTexture(Texture *texture);
void UnbindTexture();