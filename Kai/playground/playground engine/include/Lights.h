#pragma once

#include <Vertex.h>

struct AmbientLight
{
	Color LightColor = Color{1, 1, 1, 1};
	float LightStrength = 0.9f;
};
