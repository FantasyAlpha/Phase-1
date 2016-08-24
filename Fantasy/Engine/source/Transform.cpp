#include "Transform.h"

void Transform::Update()
{
	if (OldPos != vec3f())
	{
		OldPos = Position;
		OldRot = Rotation;
		OldScale = Scale;
	}
	else
	{
		OldPos = Position + vec3f(1, 1, 1);
		OldRot = Rotation * 0.5f;
		OldPos = Scale + vec3f(1, 1, 1);
	}
}

bool Transform::HasChanged()
{
	if (Parent != 0)
	{
		if (Parent->HasChanged())
		{
			return true;
		}
	}

	if (Position != OldPos)
	{
		return true;
	}

	if (Rotation != OldRot)
	{
		return true;
	}

	if (Scale != OldScale)
	{
		return true;
	}

	return false;
}

mat4f Transform::ModelMatrix()
{
	mat4f trans = Mat4::Translation(Position);
	mat4f rot = Mat4::Rotation(Rotation);
	mat4f scale = Mat4::Scale(Scale);

	return GetParentModelMatrix() * trans * rot * scale;
}

mat4f Transform::GetParentModelMatrix()
{
	if (Parent != 0)
	{
		if (Parent->HasChanged())
		{
			ParentMatrix = Parent->ModelMatrix();
		}
	}

	return ParentMatrix;
}

mat4f CalcModelMatrix(Transform *transform)
{
	mat4f trans = Mat4::Translation(transform->Position);
	mat4f rot = Mat4::Rotation(transform->Rotation);
	mat4f scale = Mat4::Scale(transform->Scale);

	return trans * rot * scale;
}

mat4f CalcLookAtViewMatrix(BasicCamera *cam)
{	
	mat4f trans = Mat4::Translation(-1.0f * cam->Eye);
	mat4f scale = Mat4::Scale(cam->Scale);
	return  scale * (Mat4::LookAt(cam->Target, cam->Up) * trans);
}

mat4f CalcFPSViewMatrix(BasicCamera *cam, float pitch, float yaw)
{	
	return mat4f();
}

mat4f CalcOrthoProjectionMatrix(float width, float height, float _near, float _far)
{
	return Mat4::Orthographic(width, height, _near, _far);
}

mat4f CalcPerspectiveProjection(float fov, float width, float height, float _near, float _far)
{
	return Mat4::Perspective(fov, width, height, _near, _far);
}

mat4f CalcProjection(BasicCamera *cam)
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

mat4f CalcMVP(Transform *transform, BasicCamera *cam)
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

bool CheckWindowHorizontalEdges(vec3f center, vec2f size, vec3f target, vec2f targetSize)
{
	if ((target.X + (targetSize.X / 2.0f)) > (center.X + (size.X / 2.0f)))
	{
		return true;
	}
	else if ((target.X - (targetSize.X / 2.0f)) < (center.X - (size.X / 2.0f)))
	{
		return true;
	}
	
	return false;
}

bool CheckWindowVerticalEdges(vec3f center, vec2f size, vec3f target, vec2f targetSize)
{
	if ((target.Y + (targetSize.Y / 2.0f)) > (center.Y + (size.Y / 2.0f)))
	{
		return true;
	}
	else if ((target.Y - (targetSize.Y / 2.0f)) < (center.Y - (size.Y / 2.0f)))
	{
		return true;
	}

	return false;
}

bool CheckWindowCenter(vec3f center, vec3f target)
{
	vec3f distance = center - target;

	float squareLength = Vec3::LengthSquare(distance);

	if ((int)(squareLength) > 0)
	{
		return true;
	}

	return false;
}

void MoveCamera(BasicCamera *cam, vec2f velocity)
{
	cam->Velocity = velocity;
	cam->Eye.X += cam->Velocity.X;
	cam->Eye.Y += cam->Velocity.Y;
}

void AutoMoveCameraHorizontal(BasicCamera *cam, vec2f velocity, vec3f target, vec2f targetSize)
{
	if (CheckWindowHorizontalEdges(cam->Eye, cam->WindowSize, target, targetSize))
	{
		cam->Velocity = velocity;
		cam->Attached = true;
	}	

	float test1 = cam->InitialEye.X;
	test1 += cam->Velocity.X;
	float test2 = test1;
	test2 += cam->Velocity.X;

	bool right = test2 > test1;

	if (right)
	{
		if (cam->Eye.X >= target.X)
		{
			if (cam->Attached)
			{
				cam->Attached = false;
				cam->Velocity = vec2f();
			}
		}
		else
		{
			if (cam->Attached)
			{
				cam->Velocity = velocity;
				cam->Eye.X += cam->Velocity.X;
			}
		}
	}
	else
	{
		if (cam->Eye.X <= target.X)
		{
			if (cam->Attached)
			{
				cam->Attached = false;
				cam->Velocity = vec2f();
			}
		}
		else
		{
			if (cam->Attached)
			{
				cam->Velocity = velocity;
				cam->Eye.X += cam->Velocity.X;
			}
		}
	}	
}

void AutoMoveCameraVertical(BasicCamera *cam, vec2f velocity, vec3f target, vec2f targetSize)
{
	if (CheckWindowHorizontalEdges(cam->Eye, cam->WindowSize, target, targetSize))
	{
		cam->Velocity = velocity;
		cam->Attached = true;
	}

	float test1 = cam->InitialEye.Y;
	test1 += cam->Velocity.Y;
	float test2 = test1;
	test2 += cam->Velocity.Y;

	bool right = test2 > test1;

	if (right)
	{
		if (cam->Eye.Y >= target.Y)
		{
			if (cam->Attached)
			{
				cam->Attached = false;
				cam->Velocity = vec2f();
			}
		}
		else
		{
			if (cam->Attached)
			{
				cam->Velocity = velocity;
				cam->Eye.Y += cam->Velocity.Y;
			}
		}
	}
	else
	{
		if (cam->Eye.Y <= target.Y)
		{
			if (cam->Attached)
			{
				cam->Attached = false;
				cam->Velocity = vec2f();
			}
		}
		else
		{
			if (cam->Attached)
			{
				cam->Velocity = velocity;
				cam->Eye.Y += cam->Velocity.Y;
			}
		}
	}
}