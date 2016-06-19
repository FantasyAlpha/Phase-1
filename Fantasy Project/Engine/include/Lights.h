#pragma once

#include <Vertex.h>

struct AmbientLight
{
	vec4f LightColor = vec4f{ 1, 1, 1, 1 };
	float LightStrength = 0.9f;
};
