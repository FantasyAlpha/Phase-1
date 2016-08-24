/*=======================================================================================================================
NOTE(kai): This file contain:

1) Vertex data structure
2) Color (R, G, B, A) data structure
=======================================================================================================================*/

#pragma once
#include <Cinder\CinderMath.h>

using namespace Cinder::Math;

struct Vertex
{
	Vertex(vec3f pos = vec3f(), vec2f texCoords = vec2f(), vec4f color = vec4f(), vec3f normal = vec3f(), float slot = 0) :
		Pos(pos), TexCoords(texCoords), Color(color), Normal(normal) , TextureSlot(slot)
	{}
	vec3f Pos;
	vec2f TexCoords;
	vec4f Color;
	float TextureSlot;
	vec3f Normal;
};