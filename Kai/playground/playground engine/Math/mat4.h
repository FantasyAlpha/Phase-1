#pragma once

#include "vec3.h"
#include "MathFunctions.h"

struct mat4
{
public:
	//Sets either the whole matrix or only the diagonal by the desired value
	mat4(float value = 1, bool diagonalOnly = true)
	{
		if (diagonalOnly)
		{
			elements[0 ] = value;		elements[1 ] = 0;			elements[2 ] = 0;			elements[3 ] = 0;
			elements[4 ] = 0;			elements[5 ] = value;		elements[6 ] = 0;			elements[7 ] = 0;
			elements[8 ] = 0;			elements[9 ] = 0;			elements[10] = value;		elements[11] = 0;
			elements[12] = 0;			elements[13] = 0;			elements[14] = 0;			elements[15] = value;
		}
		else
		{
			elements[0] = value;		elements[1] = value;		elements[2] = value;		elements[3] = value;
			elements[4] = value;		elements[5] = value;		elements[6] = value;		elements[7] = value;
			elements[8] = value;		elements[9] = value;		elements[10] = value;		elements[11] = value;
			elements[12] = value;		elements[13] = value;		elements[14] = value;		elements[15] = value;
		
		}
	}

public:
	//Get the identity matrix
	static mat4 Identity();
	
	//Multiply 2 matrices using the '*' operator
	friend mat4 operator*(mat4 &left, const mat4& right);
	//Multiply 2 matrices using the '*=' operator
	mat4 operator*=(const mat4& other);

	//NOTE(kai): this function calculate the wanted directions (forward, up, right)
	//			,then calls the overloaded rotation function to rotate the object to the desired position
	
	//Rotate the object to the wanted directions 
	//static mat4 rotate(const vec3 &forward, const vec3 &up);

	float *GetElemets(){ return elements; };

	//Translate the object by the desired amount in the desired directions (X, Y, Z)
	static mat4 Translate(vec3& amount);
	//Rotate the object by the desired angle in the desired directions (X, Y, Z)
	static mat4 Rotate(vec3 &angle);
	//Scale the object by the desired amount in the desired directions (X, Y, Z)
	static mat4 Scale(vec3& amount);

	static mat4 LookAtMatrix(vec3 &eye, vec3 &target, vec3 &up);
	static mat4 FPSMatrix(vec3 &eye, float pitch, float yaw);
	
	static mat4 OrthographicMatrix(float left, float right, float top, float bottom, float near, float far);
	static mat4 OrthographicMatrix(float width, float height, float near, float far);

	static mat4 PerspectiveMatrix(float fieldOfView, float width, float height, float near, float far);
private:
	//Rotate the object to the wanted (forward, up, right) directions 
	//static mat4 rotation(const vec3 &forward, const vec3 &up, const vec3 &right);

private:
	//Matrix elements
	float elements[4 * 4];
};