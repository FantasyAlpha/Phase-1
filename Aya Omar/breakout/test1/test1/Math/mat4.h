#pragma once

#include "vec3.h"

struct mat4
{
public:
	mat4();
	//Sets either the whole matrix or only the diagonal by the desired value
	mat4(float value, bool diagonalOnly);
	~mat4();

public:
	//Get the identity matrix
	static mat4 identity();

	//Multiply 2 matrices
	mat4& mul(const mat4& other);
	//Multiply 2 matrices using the '*' operator
	friend mat4 operator*(mat4 left, const mat4& right);
	//Multiply 2 matrices using the '*=' operator
	mat4& operator*=(const mat4& other);

	//NOTE(kai): this function calculate the wanted directions (forward, up, right)
	//			,then calls the overloaded rotation function to rotate the object to the desired position
	
	//Rotate the object to the wanted directions 
	static mat4 rotation(const vec3 &forward, const vec3 &up);

	//Translate the object by the desired amount in the desired directions (X, Y, Z)
	static mat4 translation(const vec3& amount);
	//Rotate the object by the desired angle in the desired directions (X, Y, Z)
	static mat4 rotation(const vec3 &angle);
	//Scale the object by the desired amount in the desired directions (X, Y, Z)
	static mat4 scale(const vec3& amount);

private:
	//Rotate the object to the wanted (forward, up, right) directions 
	static mat4 rotation(const vec3 &forward, const vec3 &up, const vec3 &right);

private:
	//Matrix elements
	float elements[4 * 4];
};