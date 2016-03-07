/*=======================================================================================================================
NOTE(kai): This file contain:

1) Vertex data structure
2) Position (x, y, z) data structure
3) Color (R, G, B) data structure
=======================================================================================================================*/

#pragma once
#include "Math\vec3.h"
#include "Math\vec2.h"

struct Color
{
	Color(float r = 0, float g = 0, float b = 0, float a = 0)
	{
		this->red = r;
		this->green = g;
		this->blue = b;
		this->alpha = a;		
	}

	float red;
	float green;
	float blue;
	float alpha;
};

struct Vertex
{
	Vertex(vec3 pos = vec3(0.0f, 0.0f, 0.0f), Color color = Color(0, 0, 0, 0), vec2 texCoords = vec2(0, 0))
	{
		this->pos = pos;
		this->color = color;
		this->texCoords = texCoords;
	}

	vec3 pos;
	Color color;
	vec2 texCoords;
};