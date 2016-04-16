#pragma once

#include <Texture.h>
#include <Lights.h>

struct Material
{
	Texture MeshTexture;
	Color MeshColor;
	AmbientLight BaseLight;
};