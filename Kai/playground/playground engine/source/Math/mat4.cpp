#include "Math\mat4.h"

mat4 mat4::Identity()
{
	mat4 result;

	result.elements[0][0] = 1;		result.elements[0][1] = 0;		result.elements[0][2] = 0;		result.elements[0][3] = 0;
	result.elements[1][0] = 0;		result.elements[1][1] = 1;		result.elements[1][2] = 0;		result.elements[1][3] = 0;
	result.elements[2][0] = 0;		result.elements[2][1] = 0;		result.elements[2][2] = 1;		result.elements[2][3] = 0;
	result.elements[3][0] = 0;		result.elements[3][1] = 0;		result.elements[3][2] = 0;		result.elements[3][3] = 1;

	return result;
}

//Translate the object by the desired amount in the desired directions (X, Y, Z)
mat4 mat4::Translation(vec3& amount)
{
	mat4 result;
	
	result.elements[0][0] = 1;		result.elements[0][1] = 0;		result.elements[0][2] = 0;		result.elements[0][3] = amount.x;
	result.elements[1][0] = 0;		result.elements[1][1] = 1;		result.elements[1][2] = 0;		result.elements[1][3] = amount.y;
	result.elements[2][0] = 0;		result.elements[2][1] = 0;		result.elements[2][2] = 1;		result.elements[2][3] = amount.z;
	result.elements[3][0] = 0;		result.elements[3][1] = 0;		result.elements[3][2] = 0;		result.elements[3][3] = 1;

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

	rotX.elements[0][0] = 1;		rotX.elements[0][1] = 0;		rotX.elements[0][2] = 0;		rotX.elements[0][3] = 0;
	rotX.elements[1][0] = 0;		rotX.elements[1][1] = cosf(x);	rotX.elements[1][2] = -sinf(x);	rotX.elements[1][3] = 0;
	rotX.elements[2][0] = 0;		rotX.elements[2][1] = sinf(x);	rotX.elements[2][2] = cosf(x);	rotX.elements[2][3] = 0;
	rotX.elements[3][0] = 0;		rotX.elements[3][1] = 0;		rotX.elements[3][2] = 0;		rotX.elements[3][3] = 1;
	
	rotY.elements[0][0] = cosf(y);	rotY.elements[0][1] = 0;		rotY.elements[0][2] = -sinf(y);	rotY.elements[0][3] = 0;
	rotY.elements[1][0] = 0;		rotY.elements[1][1] = 1;		rotY.elements[1][2] = 0;		rotY.elements[1][3] = 0;
	rotY.elements[2][0] = sinf(y);	rotY.elements[2][1] = 0;		rotY.elements[2][2] = cosf(y);	rotY.elements[2][3] = 0;
	rotY.elements[3][0] = 0;		rotY.elements[3][1] = 0;		rotY.elements[3][2] = 0;		rotY.elements[3][3] = 1;
	
	rotZ.elements[0][0] = cosf(z);	rotZ.elements[0][1] = -sinf(z);	rotZ.elements[0][2] = 0;		rotZ.elements[0][3] = 0;
	rotZ.elements[1][0] = sinf(z);	rotZ.elements[1][1] = cosf(z);	rotZ.elements[1][2] = 0;		rotZ.elements[1][3] = 0;
	rotZ.elements[2][0] = 0;		rotZ.elements[2][1] = 0;		rotZ.elements[2][2] = 1;		rotZ.elements[2][3] = 0;
	rotZ.elements[3][0] = 0;		rotZ.elements[3][1] = 0;		rotZ.elements[3][2] = 0;		rotZ.elements[3][3] = 1;

	result = rotZ * rotY * rotX;

	return result;
}

//Scale the object by the desired amount in the desired directions (X, Y, Z)
mat4 mat4::Scale(vec3& amount)
{
	mat4 result;

	result.elements[0][0] = amount.x;	result.elements[0][1] = 0;			result.elements[0][2] = 0;			result.elements[0][3] = 0;
	result.elements[1][0] = 0;			result.elements[1][1] = amount.y;	result.elements[1][2] = 0;			result.elements[1][3] = 0;
	result.elements[2][0] = 0;			result.elements[2][1] = 0;			result.elements[2][2] = amount.z;	result.elements[2][3] = 0;
	result.elements[3][0] = 0;			result.elements[3][1] = 0;			result.elements[3][2] = 0;			result.elements[3][3] = 1;


	return result;
}

//Multiply 2 matrices using the '*' operator
mat4 operator*(mat4 &left, const mat4& right)
{
	mat4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.elements[i][j] =
				(left.elements[i][0] * right.elements[0][j]) +
				(left.elements[i][1] * right.elements[1][j]) +
				(left.elements[i][2] * right.elements[2][j]) +
				(left.elements[i][3] * right.elements[3][j]);
		}
	}

	return result;
}

vec3 operator*(mat4 &right, vec3 &left)
{
	float res[4] = { 0, 0, 0, 1.0f };
	float newLeft[4] = { 0, 0, 0, 1.0f };
	newLeft[0] = left.x;
	newLeft[1] = left.y;
	newLeft[2] = left.z;
	newLeft[3] = 1.0f;

	for (int i = 0; i < 4; i++)
	{
		res[i] =
			right.elements[i][0] * newLeft[0] +
			right.elements[i][1] * newLeft[1] +
			right.elements[i][2] * newLeft[2] +
			right.elements[i][3] * newLeft[3];
	}

	return vec3(res[0], res[1], res[2]);
}

//Multiply 2 matrices using the '*=' operator
mat4 mat4::operator*=(const mat4& other)
{
	mat4 result;

	result = *this * other;

	return result;
}

mat4 mat4::LookAtMatrix(vec3 &target, vec3 &up)
{
	vec3 u, v, n;

	n = target; 
	n.Normalize();
	
	u = up; 
	u.Normalize();
	u = u.Cross(n);
	
	v = u.Cross(n);

	mat4 result;

	result.elements[0][0] = u.x;		result.elements[0][1] = u.y;		result.elements[0][2] = u.z;		result.elements[0][3] = 0;
	result.elements[1][0] = v.x;		result.elements[1][1] = v.y;		result.elements[1][2] = v.z;		result.elements[1][3] = 0;
	result.elements[2][0] = n.x;		result.elements[2][1] = n.y;		result.elements[2][2] = n.z;		result.elements[2][3] = 0;
	result.elements[3][0] = 0;			result.elements[3][1] = 0;			result.elements[3][2] = 0;			result.elements[3][3] = 1;

	return result;
}

mat4 mat4::FPSMatrix(vec3 &eye, float pitch, float yaw)
{
	return mat4();
}

mat4 mat4::OrthographicMatrix(float right, float left, float top, float bottom, float near, float far)
{
	mat4 result;

	result.elements[0][0] = (2.0f / (right - left));	result.elements[0][1] = 0;							result.elements[0][2] = 0;							result.elements[0][3] = -((right + left) / (right - left));
	result.elements[1][0] = 0;							result.elements[1][1] = (2.0f / (top - bottom));	result.elements[1][2] = 0;							result.elements[1][3] = -((top + bottom) / (top - bottom));
	result.elements[2][0] = 0;							result.elements[2][1] = 0;							result.elements[2][2] = (-2.0f / (far - near));		result.elements[2][3] = -((far + near) / (far - near));
	result.elements[3][0] = 0;							result.elements[3][1] = 0;							result.elements[3][2] = 0;							result.elements[3][3] = 1;


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
	float ar = width / height;
	float tanHalfFOV = tanf(ToRadians((fieldOfView) / 2.0f));
	float top = near * tanHalfFOV;
	float bottom = -top;
	float right = ar * top;//(top - bottom) / 2.0f;
	float left = -right;

	mat4 result;
	/*
	*/
	result.elements[0][0] = ((2.0f * near) / (right - left));	result.elements[0][1] = 0;									result.elements[0][2] = ((right + left) / (right - left));		result.elements[0][3] = 0;
	result.elements[1][0] = 0;									result.elements[1][1] = ((2.0f * near) / (top - bottom));	result.elements[1][2] = ((top + bottom) / (top - bottom));		result.elements[1][3] = 0;
	result.elements[2][0] = 0;									result.elements[2][1] = 0;									result.elements[2][2] = -((far + near) / (far - near));			result.elements[2][3] = -((2.0f * far * near) / (far - near));
	result.elements[3][0] = 0;									result.elements[3][1] = 0;									result.elements[3][2] = -1;										result.elements[3][3] = 0;
	
	return result;
}
