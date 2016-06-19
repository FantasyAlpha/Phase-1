#pragma once

#include "vec3f.h"
#include "MathFunctions.h"

struct vec4
{
	float x;
	float y;
	float z;
	float w;
};

struct mat4
{
public:
	//Sets either the whole matrix or only the diagonal by the desired value
	mat4(float value = 1, bool diagonalOnly = true)
	{
		if (diagonalOnly)
		{
			elements[0][0] = value;		elements[0][1] = 0;			elements[0][2] = 0;			elements[0][3] = 0;
			elements[1][0] = 0;			elements[1][1] = value;		elements[1][2] = 0;			elements[1][3] = 0;
			elements[2][0] = 0;			elements[2][1] = 0;			elements[2][2] = value;		elements[2][3] = 0;
			elements[3][0] = 0;			elements[3][1] = 0;			elements[3][2] = 0;			elements[3][3] = value;
		}
		else
		{
			elements[0][0] = value;		elements[0][1] = value;		elements[0][2] = value;		elements[0][3] = value;
			elements[1][0] = value;		elements[1][1] = value;		elements[1][2] = value;		elements[1][3] = value;
			elements[2][0] = value;		elements[2][1] = value;		elements[2][2] = value;		elements[2][3] = value;
			elements[3][0] = value;		elements[3][1] = value;		elements[3][2] = value;		elements[3][3] = value;
		
		}
	}

public:
	//Get the identity matrix
	static mat4 Identity();
	
	//Multiply 2 matrices using the '*' operator
	friend mat4 operator*(mat4 &left, const mat4& right);
	//Multiply 2 matrices using the '*=' operator
	mat4 operator*=(const mat4& other);

	friend vec3f operator*(mat4 &left, vec3f& right);

	//NOTE(kai): this function calculate the wanted directions (forward, up, right)
	//			,then calls the overloaded rotation function to rotate the object to the desired vec3f
	
	//Rotate the object to the wanted directions 
	//static mat4 rotate(const vec3f &forward, const vec3f &up);

	float *GetElemets(){ return &elements[0][0]; };

	//Translate the object by the desired amount in the desired directions (X, Y, Z)
	static mat4 Translation(vec3f& amount);
	//Rotate the object by the desired angle in the desired directions (X, Y, Z)
	static mat4 Rotation(vec3f &angle);
	//Scale the object by the desired amount in the desired directions (X, Y, Z)
	static mat4 Scale(vec3f& amount);

	static mat4 LookAtMatrix(vec3f &target, vec3f &up);
	static mat4 FPSMatrix(vec3f &eye, float pitch, float yaw);
	
	static mat4 OrthographicMatrix(float left, float right, float top, float bottom, float near, float far);
	static mat4 OrthographicMatrix(float width, float height, float near, float far);

	static mat4 PerspectiveMatrix(float fieldOfView, float width, float height, float near, float far);
private:
	//Rotate the object to the wanted (forward, up, right) directions 
	//static mat4 rotation(const vec3f &forward, const vec3f &up, const vec3f &right);

private:
	//Matrix elements
	//float elements[4 * 4];
	float elements[4][4];
};