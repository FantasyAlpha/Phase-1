/*=======================================================================================================================
NOTE(kai): This file contain:

1) Vertex data structure
2) Color (R, G, B, A) data structure
=======================================================================================================================*/

#pragma once
#include <Math\vec3.h>
#include <Math\vec2.h>

struct Color
{
	float red;
	float green;
	float blue;
	float alpha;
};

struct Vertex
{
	vec3 Pos;
	vec2 TexCoords;
	vec3 Normal;
};