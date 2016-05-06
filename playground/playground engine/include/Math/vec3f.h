#pragma once

#include "MathFunctions.h"

struct vec3f
{
public:
	vec3f(float x = 0, float y = 0, float z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	~vec3f(){}
public:
	//Get vector length
	float Length();

	//Get vector length ^ 2
	float LengthSquare();

	//Normalize the vector
	vec3f Normalize();

	vec3f Cross(vec3f &other);
	float Dot(vec3f &other);
	   
	//NOTE(kai): if i'm doing an operation on an instance of this class
	//			,and the instance exist on the right hand side, then i must 
	//			use the overloaded operator function as a separate friend function (not part of the class)

	//Add 2 vectors using the '+' operator
	friend vec3f operator+(vec3f &left, vec3f &right);
	//Subtract 2 vectors using the '-' operator
	friend vec3f operator-(vec3f &left, vec3f &right);
	   		
	//Multiply 2 vectors using the '*' operator
	friend vec3f operator*(vec3f &left, vec3f &right);
	//Multiply vector with a constant value using the '*' operator
	friend vec3f operator*(vec3f left, float right);
	friend vec3f operator*(float left, vec3f right);
	   		
	//Divide 2 vectors using the '/' operator
	friend vec3f operator/(vec3f &left, vec3f &right);
	//Divide vector by a constant value using the '/' operator
	friend vec3f operator/(vec3f left, float right);
	friend vec3f operator/(float left, vec3f right);
	   
	//Check if 2 vectors are equal
	bool operator==(vec3f &other);
	//Check if 2 vectors are not equal
	bool operator!=(vec3f &other);
	   
	//Add 2 vectors using the '+=' operator
	vec3f operator+=(vec3f &other);
	//Subtract 2 vectors using the '-=' operator
	vec3f operator-=(vec3f &other);
	   
	//Multiply 2 vectors using the '*=' operator
	vec3f operator*=(vec3f &other);
	//Multiply vector with a constant value using the '*=' operator
	vec3f operator*=(float &other);
	   
	//Divide 2 vectors using the '/=' operator
	vec3f operator/=(vec3f &other);
	//Divide vector by a constant value using the '/=' operator
	vec3f operator/=(float &other);

public:
	//Vector data
	float x, y, z;
};

