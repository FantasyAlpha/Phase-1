#pragma once

#include "Math\vec3.h"
#include "Math\mat4.h"

struct Camera
{
	Camera(vec3 eye = vec3(), vec3 scale = vec3(1, 1, 1),
		   mat4 viewMatrix = mat4(), mat4 projectionMatrix = mat4())
	{
		Eye = eye;
		Scale = scale;	
		
		ViewMatrix = viewMatrix;
		ProjectionMatrix = projectionMatrix;
	}

	vec3 Eye;
	vec3 Scale;	

	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

struct Transform
{
	Transform(vec3 Position = vec3(), vec3 Rotation = vec3(), vec3 Scale = vec3(1, 1, 1),
			  mat4 ModelMatrix = mat4())
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;

		this->ModelMatrix = ModelMatrix;
	}

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	mat4 ModelMatrix;
};

mat4 CalculateModelMatrix(Transform *transform);

mat4 CalculateLookAtViewMatrix(Camera *cam, vec3 &target, vec3 &up);
mat4 CalculateFPSViewMatrix(Camera *cam, float pitch, float yaw);

mat4 CalculateOrthoProjectionMatrix(Camera *cam, float width, float height, float near, float far);
mat4 CalculateOrthoProjectionMatrix(Camera *cam, float left, float right, float top, float bottom, float near, float far);

mat4 CalculatePerspectiveProjection(Camera *cam, float fov, float width, float height, float near, float far);

mat4 CalculateMVP(Transform *transform, Camera *cam);
