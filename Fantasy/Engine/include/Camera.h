#pragma once

#include <Cinder\CinderMath.h>

using namespace Cinder::Math;

enum CameraType
{
	ORTHOGRAPHIC = 0,
	PERSPECTIVE = 1
};

struct BasicCamera
{
public:
	BasicCamera(){}
	BasicCamera(vec2f size, vec3f eye = vec3f(), vec3f target = vec3f(0, 0, 1)
		, vec3f up = vec3f(0, -1, 0), vec3f scale = vec3f(1, 1, 1)
		, float fov = 80.0f, float far = 1000.0f
		, CameraType type = CameraType::ORTHOGRAPHIC);
	~BasicCamera(){}
public:
	vec3f Eye = vec3f();
	vec3f InitialEye;
	vec3f Target = vec3f(0, 0, 1);
	vec3f Up = vec3f(0, -1, 0);

	vec3f Scale = vec3f(1, 1, 1);

	vec2f Size;
	vec2f WindowSize;
	vec2f Velocity;

	float FOV = 120.0f;
	float Far = 100.0f;

	CameraType Type = CameraType::ORTHOGRAPHIC;

	bool Attached;
};