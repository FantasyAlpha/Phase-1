#pragma once

#include "Math\vec3.h"
#include "Math\mat4.h"

struct Transform
{
	Transform(vec3 Position = vec3(), vec3 Rotation = vec3(), vec3 Scale = vec3(1, 1, 1),
			  mat4 ModelMatrix = mat4(), mat4 ViewMatrix = mat4(), mat4 ProjectionMatrix = mat4())
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;

		this->ModelMatrix = ModelMatrix;
		this->ProjectionMatrix = ProjectionMatrix;
		this->ViewMatrix = ViewMatrix;

	}

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	mat4 ModelMatrix;
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void CalculateModelMatrix(Transform *transform);

