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
	Color(float r = 1, float g = 1, float b = 1, float a = 1):
		red(r), green(g), blue(b), alpha(a)
	{}

	float red;
	float green;
	float blue;
	float alpha;
};

struct Vertex
{
	Vertex(vec3 pos = vec3(), vec2 texCoords = vec2(), Color color = Color(), float slot = 0) :
		Pos(pos), TexCoords(texCoords), Col(color), TextureSlot(slot)
	{}
	vec3 Pos;
	vec2 TexCoords;
	Color Col;
	float TextureSlot;
	//vec3 Normal;
};