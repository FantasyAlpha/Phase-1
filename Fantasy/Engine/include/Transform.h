#pragma once

#include "Camera.h"

struct Transform
{
public:
	Transform(vec3f position = vec3f(), vec3f rotation = vec3f(), vec3f scale = vec3f(1, 1, 1))
	{
		this->Position = position;
		this->Rotation = rotation;
		this->Scale = scale;

		this->OldPos = vec3f();
		this->OldRot = vec3f();
		this->OldScale = vec3f(1, 1, 1);

		this->Parent = 0;
		this->ParentMatrix = mat4f();
	}

public:
	void Update();

	mat4f ModelMatrix();
private:
	bool HasChanged();

	mat4f GetParentModelMatrix();
public:
	vec3f Position;
	vec3f Rotation;
	vec3f Scale;
	Transform *Parent;

	vec3f OldPos;
private:
	vec3f OldRot;
	vec3f OldScale;

	mat4f ParentMatrix;
};

void Translate(Transform *transform, vec3f amount);
void Rotate(Transform *transform, vec3f amount);
void Scale(Transform *transform, vec3f amount);

mat4f CalcModelMatrix(Transform *transform);

mat4f CalcLookAtViewMatrix(BasicCamera *cam);
mat4f CalcFPSViewMatrix(BasicCamera *cam, float pitch, float yaw);

mat4f CalcOrthoProjectionMatrix(float width, float height, float _near, float _far);

mat4f CalcPerspectiveProjection(float fov, float width, float height, float _near, float _far);

mat4f CalcProjection(BasicCamera *cam);

mat4f CalcMVP(Transform *transform, BasicCamera *cam);

bool operator!=(Transform &left, Transform &right); 
bool operator==(Transform &left, Transform &right);

bool CheckWindowHorizontalEdges(vec3f center, vec2f size, vec3f target, vec2f targetSize);
bool CheckWindowVerticalEdges(vec3f center, vec2f size, vec3f target, vec2f targetSize);
bool CheckWindowCenter(vec3f center, vec3f target);
void MoveCamera(BasicCamera *cam, vec2f velocity);
void AutoMoveCameraHorizontal(BasicCamera *cam, vec2f velocity, vec3f target, vec2f targetSize);
void AutoMoveCameraVertical(BasicCamera *cam, vec2f velocity, vec3f target, vec2f targetSize);