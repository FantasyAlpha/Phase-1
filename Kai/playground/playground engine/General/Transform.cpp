#include "Transform.h"

mat4 ModelMatrix(Transform *transform)
{
	mat4 pos = mat4::Translation(transform->Position);
	mat4 rot = mat4::Rotation(transform->Rotation);
	mat4 scale = mat4::Scale(transform->Scale);

	return pos * rot * scale;
}

mat4 LookAtViewMatrix(Camera *cam, vec3 &target, vec3 &up)
{	
	return mat4::LookAtMatrix(cam->Eye, target, up) * mat4::Scale(cam->Scale);
}

mat4 FPSViewMatrix(Camera *cam, float pitch, float yaw)
{	
	return mat4::FPSMatrix(cam->Eye, pitch, yaw) * mat4::Scale(cam->Scale);
}

mat4 OrthoProjectionMatrix(Camera *cam, float width, float height, float near, float far)
{
	return mat4::OrthographicMatrix(width, height, near, far);
}

mat4 OrthoProjectionMatrix(Camera *cam, float left, float right, float top, float bottom, float near, float far)
{
	return mat4::OrthographicMatrix(left, right, top, bottom, near, far);
}

mat4 PerspectiveProjection(Camera *cam, float fov, float width, float height, float near, float far)
{
	return mat4::PerspectiveMatrix(fov, width, height, near, far);
}

mat4 MVP(mat4 model, mat4 view, mat4 projection)
{
	return projection * view * model;
}

void Translate(Transform *transform, vec3 amount)
{
	mat4 transMat = mat4::Translation(amount);

	transform->Position = transform->Position * transMat;
}

void Rotate(Transform *transform, vec3 amount)
{
	mat4 rotMat = mat4::Rotation(amount);

	transform->Rotation = transform->Rotation * rotMat;
}

void Scale(Transform *transform, vec3 amount)
{
	mat4 scaleMat = mat4::Scale(amount);

	transform->Scale = transform->Scale * scaleMat;
}

bool operator!=(Transform &left, Transform &right)
{
	bool pos = left.Position == right.Position;
	bool rot = left.Rotation == right.Rotation;
	bool scale = left.Scale == right.Scale;

	return pos || rot || scale;
}

bool operator==(Transform &left, Transform &right)
{
	bool pos = left.Position == right.Position;
	bool rot = left.Rotation == right.Rotation;
	bool scale = left.Scale == right.Scale;

	return pos && rot && scale;
}