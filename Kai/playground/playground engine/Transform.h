#pragma once

#include "Math\vec3.h"
#include "Math\mat4.h"

#define LOOK_AT_CAMERA 1
#define FPS_CAMERA 2

#define ORTHOGRAPHIC_PROJECTION 1
#define PERSPECTIVE_PROJECTION 2

struct ProjectionFrustum
{
	ProjectionFrustum(float fieldOfView = 0
					, float near = 0, float far = 0
					, float width = 0, float height = 0
					, int projectionType = ORTHOGRAPHIC_PROJECTION
					, mat4 projectionMatrix = mat4())		
		: FieldOfView(fieldOfView)
		, Near(near), Far(far)
		, Width(width), Height(height)
		, ProjectionType(projectionType)
		, ProjectionMatrix(projectionMatrix)
	{
		
	}

	float FieldOfView;
	float Near;
	float Far;
	float Width;
	float Height;

	int ProjectionType;

	mat4 ProjectionMatrix;
};

struct Camera
{
	Camera(int cameraType = LOOK_AT_CAMERA, vec3 eye = vec3(), vec3 scale = vec3(1, 1, 1),
		   mat4 viewMatrix = mat4())
	{
		Eye = eye;
		Scale = scale;	
		
		ViewMatrix = viewMatrix;

		CameraType = cameraType;
	}

	vec3 Eye;
	vec3 Scale;	

	mat4 ViewMatrix;

	int CameraType;	
};

struct Transform
{
	Transform(vec3 Position = vec3(), vec3 Rotation = vec3(), vec3 Scale = vec3(1, 1, 1),
			  mat4 ModelMatrix = mat4())
	{
		this->Position = Position;
		this->Rotation = Rotation;
		this->Scale = Scale;

		this->ModelMatrix = ModelMatrix;
	}

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	mat4 ModelMatrix;
};

void CalculateModelMatrix(Transform *transform);

void CalculateLookAtViewMatrix(Camera *cam, vec3 &target, vec3 &up);
void CalculateFPSViewMatrix(Camera *cam, float pitch, float yaw);

void CalculateProjectionMatrix(ProjectionFrustum *projection);

mat4 CalculateMVP(Transform *transform, Camera *cam, ProjectionFrustum *projection);