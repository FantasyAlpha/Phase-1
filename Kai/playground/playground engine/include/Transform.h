#pragma once

#include <Math\vec3.h>
#include <Math\vec2.h>
#include <Math\mat4.h>

enum CameraType
{
	ORTHOGRAPHIC = 0, 
	PERSPECTIVE = 1
};

struct Camera
{
	Camera(){}
	Camera(vec2 size, vec3 eye = vec3(), vec3 target = vec3(0, 0, 1)
		, vec3 up = vec3(0, 1, 0), vec3 scale = vec3(1, 1, 1)
		, float fov = 120.0f, float _far = 100.0f
		, CameraType type = CameraType::ORTHOGRAPHIC)
	{
		Size = size;
		Eye = eye;
		Target = target;
		Up = up;
		Scale = scale;
		FOV = fov;
		Far = _far;
		Type = type;
	}

	vec2 Size;

	vec3 Eye;
	vec3 Target;
	vec3 Up;

	vec3 Scale;	

	float FOV;
	float Far;

	CameraType Type;
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

mat4 CalcModelMatrix(Transform *transform);

mat4 CalcLookAtViewMatrix(Camera *cam);
mat4 CalcFPSViewMatrix(Camera *cam, float pitch, float yaw);

mat4 CalcOrthoProjectionMatrix(float width, float height, float near, float far);

mat4 CalcPerspectiveProjection(float fov, float width, float height, float near, float far);

mat4 CalcProjection(Camera *cam);

mat4 CalcMVP(Transform *transform, Camera *cam);

bool operator!=(Transform &left, Transform &right); 
bool operator==(Transform &left, Transform &right);