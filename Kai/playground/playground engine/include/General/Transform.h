#pragma once

#include "..\Math\vec3.h"
#include "..\Math\mat4.h"

struct Camera
{
	Camera(vec3 eye = vec3(), vec3 scale = vec3(1, 1, 1))
	{
		Eye = eye;
		Scale = scale;	
	}

	vec3 Eye;
	vec3 Scale;	
};

struct Transform
{
	Transform(vec3 Position = vec3(), vec3 Rotation = vec3(), vec3 Scale = vec3(1, 1, 1),
			  mat4 ModelMatrix = mat4())
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;
	}

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;
};

void Translate(Transform *transform, vec3 amount);
void Rotate(Transform *transform, vec3 amount);
void Scale(Transform *transform, vec3 amount);

mat4 ModelMatrix(Transform *transform);

mat4 LookAtViewMatrix(Camera *cam, vec3 &target, vec3 &up);
mat4 FPSViewMatrix(Camera *cam, float pitch, float yaw);

mat4 OrthoProjectionMatrix(Camera *cam, float width, float height, float near, float far);
mat4 OrthoProjectionMatrix(Camera *cam, float left, float right, float top, float bottom, float near, float far);

mat4 PerspectiveProjection(Camera *cam, float fov, float width, float height, float near, float far);

mat4 MVP(mat4 model, mat4 view, mat4 projection);

bool operator!=(Transform &left, Transform &right); 
bool operator==(Transform &left, Transform &right);