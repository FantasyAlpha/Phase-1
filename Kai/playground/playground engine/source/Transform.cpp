#include <Transform.h>

mat4 CalcModelMatrix(Transform *transform)
{
	mat4 pos = mat4::Translation(transform->Position);
	mat4 rot = mat4::Rotation(transform->Rotation);
	mat4 scale = mat4::Scale(transform->Scale);

	return pos * rot * scale;
}

mat4 CalcLookAtViewMatrix(Camera *cam)
{	
	mat4 camTrans = mat4::Translation(cam->Eye * -1);
	return (mat4::Scale(cam->Scale) * mat4::LookAtMatrix(cam->Target, cam->Up) * camTrans);
}

mat4 CalcFPSViewMatrix(Camera *cam, float pitch, float yaw)
{	
	return mat4::FPSMatrix(cam->Eye, pitch, yaw) * mat4::Scale(cam->Scale);
}

mat4 CalcOrthoProjectionMatrix(float width, float height, float near, float far)
{
	return mat4::OrthographicMatrix(width, height, near, far);
}

mat4 CalcPerspectiveProjection(float fov, float width, float height, float near, float far)
{
	return mat4::PerspectiveMatrix(fov, width, height, near, far);
}

mat4 CalcProjection(Camera *cam)
{
	mat4 result;
	mat4 proj;

	if (cam->Type == CameraType::ORTHOGRAPHIC)
	{
		proj = CalcOrthoProjectionMatrix(cam->Size.x, cam->Size.y, cam->Eye.z, cam->Far);
	}
	else
	{
		proj = CalcPerspectiveProjection(cam->FOV, cam->Size.x, cam->Size.y, cam->Eye.z + 0.1f, cam->Far);
	}

	result = proj * CalcLookAtViewMatrix(cam);

	return result;
}

mat4 CalcMVP(Transform *transform, Camera *cam)
{
	return CalcProjection(cam) * CalcLookAtViewMatrix(cam) * CalcModelMatrix(transform);
}

void Translate(Transform *transform, vec3 amount)
{
	mat4 transMat = mat4::Translation(amount);

	transform->Position = transMat * transform->Position;
}

void Rotate(Transform *transform, vec3 amount)
{
	mat4 rotMat = mat4::Rotation(amount);

	transform->Rotation = rotMat * transform->Rotation;
}

void Scale(Transform *transform, vec3 amount)
{
	mat4 scaleMat = mat4::Scale(amount);

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