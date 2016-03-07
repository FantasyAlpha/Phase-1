#include "Transform.h"

mat4 CalculateModelMatrix(Transform *transform)
{
	mat4 pos = mat4::Translate(transform->Position);
	mat4 rot = mat4::Rotate(transform->Rotation);
	mat4 scale = mat4::Scale(transform->Scale);

	transform->ModelMatrix = pos * rot * scale;

	return transform->ModelMatrix;
}

mat4 CalculateLookAtViewMatrix(Camera *cam, vec3 &target, vec3 &up)
{
	cam->ViewMatrix = mat4::LookAtMatrix(cam->Eye, target, up) * mat4::Scale(cam->Scale);
	
	return cam->ViewMatrix;
}

mat4 CalculateFPSViewMatrix(Camera *cam, float pitch, float yaw)
{
	cam->ViewMatrix = mat4::FPSMatrix(cam->Eye, pitch, yaw) * mat4::Scale(cam->Scale);
	
	return cam->ViewMatrix;
}

mat4 CalculateOrthoProjectionMatrix(Camera *cam, float width, float height, float near, float far)
{
	cam->ProjectionMatrix = mat4::OrthographicMatrix(width, height, near, far);

	return cam->ProjectionMatrix;
}

mat4 CalculateOrthoProjectionMatrix(Camera *cam, float left, float right, float top, float bottom, float near, float far)
{
	cam->ProjectionMatrix = mat4::OrthographicMatrix(left, right, top, bottom, near, far);

	return cam->ProjectionMatrix;
}

mat4 CalculatePerspectiveProjection(Camera *cam, float fov, float width, float height, float near, float far)
{
	cam->ProjectionMatrix = mat4::PerspectiveMatrix(fov, width, height, near, far);

	return cam->ProjectionMatrix;
}

mat4 CalculateMVP(Transform *transform, Camera *cam)
{
	return cam->ProjectionMatrix * cam->ViewMatrix * transform->ModelMatrix;
}