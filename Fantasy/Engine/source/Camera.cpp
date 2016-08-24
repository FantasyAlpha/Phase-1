#include "Camera.h"

BasicCamera::BasicCamera(vec2f size, vec3f eye, vec3f target
	, vec3f up, vec3f scale
	, float fov, float far
	, CameraType type)
{
	Size = size;
	Eye = eye;
	InitialEye = eye;
	Target = target;
	Up = up;
	Scale = scale;
	FOV = fov;
	Far = far;
	Type = type;
}