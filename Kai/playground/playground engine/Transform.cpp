#include "Transform.h"

void CalculateModelMatrix(Transform *transform)
{
	mat4 pos = mat4::Translate(transform->Position);
	mat4 rot = mat4::Rotate(transform->Rotation);
	mat4 scale = mat4::Scale(transform->Scale);

	transform->ModelMatrix = pos * rot * scale;
}