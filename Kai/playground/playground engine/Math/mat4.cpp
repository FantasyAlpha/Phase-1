#include "mat4.h"

mat4 mat4::Identity()
{
	mat4 result;

	result.elements[0] = 1;		result.elements[1] = 0;			result.elements[2] = 0;			result.elements[3] = 0;
	result.elements[4] = 0;		result.elements[5] = 1;			result.elements[6] = 0;			result.elements[7] = 0;
	result.elements[8] = 0;		result.elements[9] = 0;			result.elements[10] = 1;		result.elements[11] = 0;
	result.elements[12] = 0;	result.elements[13] = 0;		result.elements[14] = 0;		result.elements[15] = 1;

	return result;
}

//Translate the object by the desired amount in the desired directions (X, Y, Z)
mat4 mat4::Translation(vec3& amount)
{
	mat4 result;

	result.elements[0] = 1;				result.elements[1] = 0;					result.elements[2] = 0;				result.elements[3] = 0;
	result.elements[4] = 0;				result.elements[5] = 1;					result.elements[6] = 0;				result.elements[7] = 0;
	result.elements[8] = 0;				result.elements[9] = 0;					result.elements[10] = 1;			result.elements[11] = 0;
	result.elements[12] = amount.x;		result.elements[13] = amount.y;			result.elements[14] = amount.z;		result.elements[15] = 1;

	return result;
}

//Rotate the object by the desired angle in the desired directions (X, Y, Z)
mat4 mat4::Rotation(vec3 &angle)
{
	mat4 rotX;
	mat4 rotY;
	mat4 rotZ;

	mat4 result;

	float x = ToRadians(angle.x);
	float y = ToRadians(angle.y);
	float z = ToRadians(angle.z);

	rotX.elements[0] = 1;		rotX.elements[1] = 0;				rotX.elements[2] = 0;				rotX.elements[3] = 0;
	rotX.elements[4] = 0;		rotX.elements[5] = cosf(x);			rotX.elements[6] = -sinf(x);		rotX.elements[7] = 0;
	rotX.elements[8] = 0;		rotX.elements[9] = sinf(x);			rotX.elements[10] = cosf(x);		rotX.elements[11] = 0;
	rotX.elements[12] = 0;		rotX.elements[13] = 0;				rotX.elements[14] = 0;				rotX.elements[15] = 1;

	rotY.elements[0] = cosf(y);		rotY.elements[1] = 0;			rotY.elements[2] = sinf(y);			rotY.elements[3] = 0;
	rotY.elements[4] = 0;			rotY.elements[5] = 1;			rotY.elements[6] = 0;				rotY.elements[7] = 0;
	rotY.elements[8] = -sinf(y);	rotY.elements[9] = 0;			rotY.elements[10] = cosf(y);		rotY.elements[11] = 0;
	rotY.elements[12] = 0;			rotY.elements[13] = 0;			rotY.elements[14] = 0;				rotY.elements[15] = 1;

	rotZ.elements[0] = cosf(z);		rotZ.elements[1] = -sinf(z);		rotZ.elements[2] = 0;			rotZ.elements[3] = 0;
	rotZ.elements[4] = sinf(z);		rotZ.elements[5] = cos(z);			rotZ.elements[6] = 0;			rotZ.elements[7] = 0;
	rotZ.elements[8] = 0;			rotZ.elements[9] = 0;				rotZ.elements[10] = 1;			rotZ.elements[11] = 0;
	rotZ.elements[12] = 0;			rotZ.elements[13] = 0;				rotZ.elements[14] = 0;			rotZ.elements[15] = 1;

	result = rotX * rotY * rotZ;

	return result;
}

//Scale the object by the desired amount in the desired directions (X, Y, Z)
mat4 mat4::Scale(vec3& amount)
{
	mat4 result;

	result.elements[0] = amount.x;		result.elements[1] = 0;					result.elements[2] = 0;					result.elements[3] = 0;
	result.elements[4] = 0;				result.elements[5] = amount.y;			result.elements[6] = 0;					result.elements[7] = 0;
	result.elements[8] = 0;				result.elements[9] = 0;					result.elements[10] = amount.z;			result.elements[11] = 0;
	result.elements[12] = 0;			result.elements[13] = 0;				result.elements[14] = 0;				result.elements[15] = 1;

	return result;
}

//Multiply 2 matrices using the '*' operator
mat4 operator*(mat4 &left, const mat4& right)
{
	mat4 result;

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			float sum = 0;

			for (int j = 0; j < 4; j++)
			{
				sum += left.elements[j + (i * 4)] * right.elements[j + (k * 4)];
			}

			result.elements[i + (k * 4)] = sum;
		}
	}

	return result;
}

vec3 operator*(vec3 left, const mat4& right)
{
	float res[4] = { 0, 0, 0, 1.0f };
	float newLeft[4] = { 0, 0, 0, 1.0f };
	newLeft[0] = left.x;
	newLeft[1] = left.y;
	newLeft[2] = left.z;
	newLeft[3] = 1.0f;

	for (int k = 0; k < 4; k++)
	{
		float sum = 0;
		for (int j = 0; j < 4; j++)
		{
			sum += newLeft[j] * right.elements[k + (j * 4)];
		}

		res[k] = sum;
	}

	return vec3(res[0], res[1], res[2]);
}

//Multiply 2 matrices using the '*=' operator
mat4 mat4::operator*=(const mat4& other)
{
	mat4 result;

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			float sum = 0;

			for (int j = 0; j < 4; j++)
			{
				sum += this->elements[j + (i * 4)] * other.elements[k + (j * 4)];
			}

			result.elements[k + (i * 4)] = sum;
		}
	}

	return result;
}

mat4 mat4::LookAtMatrix(vec3 &eye, vec3 &target, vec3 &up)
{
	vec3 forward = (eye - target).Normalize();
	vec3 right = (up.Cross(forward)).Normalize();
	up = (right.Cross(forward)).Normalize();

	mat4 result;

	result.elements[0] = right.x;				result.elements[1] = up.x;				result.elements[2] = forward.x;				result.elements[3] = 0;
	result.elements[4] = right.y;				result.elements[5] = up.y;				result.elements[6] = forward.y;				result.elements[7] = 0;
	result.elements[8] = right.z;				result.elements[9] = up.z;				result.elements[10] = forward.z;			result.elements[11] =0;
	result.elements[12] = -(eye.Dot(right));		result.elements[13] = -(eye.Dot(up));	result.elements[14] = -(eye.Dot(forward));	result.elements[15] = 1;

	return result;
}

mat4 mat4::FPSMatrix(vec3 &eye, float pitch, float yaw)
{
	return mat4();
}

mat4 mat4::OrthographicMatrix(float right, float left, float top, float bottom, float near, float far)
{
	mat4 result;

	result.elements[0] = (2.0f / (right - left));				result.elements[1] = 0;										result.elements[2] = 0;									result.elements[3] = 0;
	result.elements[4] = 0;										result.elements[5] = (2.0f / (top - bottom));				result.elements[6] = 0;									result.elements[7] = 0;
	result.elements[8] = 0;										result.elements[9] = 0;										result.elements[10] = -(2.0f / (far - near));			result.elements[11] = 0;
	result.elements[12] = -((right + left) / (right - left));	result.elements[13] = -((top + bottom) / (top - bottom));	result.elements[14] = -((far + near) / (far - near));	result.elements[15] = 1;

	return result;
}

mat4 mat4::OrthographicMatrix(float width, float height, float near, float far)
{	
	float top = height / 2.0f;
	float bottom = -top;

	float right = width / 2.0f;
	float left = -right;

	return mat4::OrthographicMatrix(right, left, top, bottom, near, far);
}

mat4 mat4::PerspectiveMatrix(float fieldOfView, float width, float height, float near, float far)
{
	float aspectRatio = width / height;
	float top = near * tanf(ToRadians(fieldOfView / 2.0f));
	float bottom = -top;
	float right = top * aspectRatio;
	float left = -right;

	mat4 result;

	result.elements[0] = ((2.0f * near) / (right - left));		result.elements[1] = 0;										result.elements[2] = 0;											result.elements[3] = 0;
	result.elements[4] = 0;										result.elements[5] = ((2.0f * near) / (top - bottom));		result.elements[6] = 0;											result.elements[7] = 0;
	result.elements[8] = ((right + left) / (right - left));		result.elements[9] = ((top + bottom) / (top - bottom));		result.elements[10] = -((far + near) / (far - near));			result.elements[11] = -1;
	result.elements[12] = 0;									result.elements[13] = 0;									result.elements[14] = -((2.0f * far * near) / (far - near));		result.elements[15] = 0;

	return result;
}
