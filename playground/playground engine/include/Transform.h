#pragma once

#include <Cinder\CinderMath.h>

using namespace Cinder::Math;

enum CameraType
{
	ORTHOGRAPHIC = 0, 
	PERSPECTIVE = 1
};

struct Camera
{
	Camera(){}
	Camera(vec2f size, vec3f eye = vec3f(), vec3f target = vec3f(0, 0, 1)
		, vec3f up = vec3f(0, 1, 0), vec3f scale = vec3f(1, 1, 1)
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

	vec2f Size;

	vec3f Eye;
	vec3f Target;
	vec3f Up;

	vec3f Scale;	

	float FOV;
	float Far;

	CameraType Type;
};

struct Transform
{
	Transform(vec3f Position = vec3f(), vec3f Rotation = vec3f(), vec3f Scale = vec3f(1, 1, 1),
			  mat4f ModelMatrix = mat4f())
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;
	}

	vec3f Position;
	vec3f Rotation;
	vec3f Scale;
};

void Translate(Transform *transform, vec3f amount);
void Rotate(Transform *transform, vec3f amount);
void Scale(Transform *transform, vec3f amount);

mat4f CalcModelMatrix(Transform *transform);

mat4f CalcLookAtViewMatrix(Camera *cam);
mat4f CalcFPSViewMatrix(Camera *cam, float pitch, float yaw);

mat4f CalcOrthoProjectionMatrix(float width, float height, float near, float far);

mat4f CalcPerspectiveProjection(float fov, float width, float height, float near, float far);

mat4f CalcProjection(Camera *cam);

mat4f CalcMVP(Transform *transform, Camera *cam);

bool operator!=(Transform &left, Transform &right); 
bool operator==(Transform &left, Transform &right);