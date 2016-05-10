#include <Transform.h>

mat4f CalcModelMatrix(Transform *transform)
{
	mat4f trans = Mat4::Translation(transform->Position);
	mat4f rot = Mat4::Rotation(transform->Rotation);
	mat4f scale = Mat4::Scale(transform->Scale);

	return trans * rot * scale;
}

mat4f CalcLookAtViewMatrix(Camera *cam)
{	
	mat4f camTrans = Mat4::Translation(cam->Eye * -1.0f);
	return (Mat4::Scale(cam->Scale) * Mat4::LookAt(cam->Target, cam->Up) * camTrans);
}

mat4f CalcFPSViewMatrix(Camera *cam, float pitch, float yaw)
{	
	return mat4f();
}

mat4f CalcOrthoProjectionMatrix(float width, float height, float near, float far)
{
	return Mat4::Orthographic(width, height, near, far);
}

mat4f CalcPerspectiveProjection(float fov, float width, float height, float near, float far)
{
	return Mat4::Perspective(fov, width, height, near, far);
}

mat4f CalcProjection(Camera *cam)
{
	mat4f result;
	mat4f proj;

	if (cam->Type == CameraType::ORTHOGRAPHIC)
	{
		proj = CalcOrthoProjectionMatrix(cam->Size.X, cam->Size.Y, cam->Eye.Z, cam->Far);
	}
	else
	{
		proj = CalcPerspectiveProjection(cam->FOV, cam->Size.X, cam->Size.Y, cam->Eye.Z + 0.1f, cam->Far);
	}

	result = proj * CalcLookAtViewMatrix(cam);

	return result;
}

mat4f CalcMVP(Transform *transform, Camera *cam)
{
	return CalcProjection(cam) * CalcLookAtViewMatrix(cam) * CalcModelMatrix(transform);
}

void Translate(Transform *transform, vec3f amount)
{
	mat4f transMat = Mat4::Translation(amount);

	transform->Position = transMat * transform->Position;
}

void Rotate(Transform *transform, vec3f amount)
{
	mat4f rotMat = Mat4::Rotation(amount);

	transform->Rotation = rotMat * transform->Rotation;
}

void Scale(Transform *transform, vec3f amount)
{
	mat4f scaleMat = Mat4::Scale(amount);

	transform->Scale = scaleMat * transform->Scale;
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