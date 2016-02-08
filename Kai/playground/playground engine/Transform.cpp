#include "Transform.h"

void CalculateModelMatrix(Transform *transform)
{
	mat4 pos = mat4::Translate(transform->Position);
	mat4 rot = mat4::Rotate(transform->Rotation);
	mat4 scale = mat4::Scale(transform->Scale);

	transform->ModelMatrix = pos * rot * scale;
}

void CalculateLookAtViewMatrix(Camera *cam, vec3 &target, vec3 &up)
{
	cam->ViewMatrix = mat4::LookAtMatrix(cam->Eye, target, up) * mat4::Scale(cam->Scale);
}

void CalculateFPSViewMatrix(Camera *cam, float pitch, float yaw)
{
	cam->ViewMatrix = mat4::FPSMatrix(cam->Eye, pitch, yaw) * mat4::Scale(cam->Scale);
}

void CalculateProjectionMatrix(ProjectionFrustum *projection)
{

}

mat4 CalculateMVP(Transform *transform, Camera *cam, ProjectionFrustum *projection)
{
	return projection->ProjectionMatrix * cam->ViewMatrix * transform->ModelMatrix;
}