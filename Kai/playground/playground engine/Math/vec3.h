#pragma once

#include "MathFunctions.h"

struct vec3
{
public:
	vec3(float x = 0, float y = 0, float z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	~vec3(){}
public:
	//Get vector length
	float Length();

	//Get vector length ^ 2
	float LengthSquare();

	//Normalize the vector
	void Normalize();

	vec3 Cross(vec3 &other);
	float Dot(vec3 &other);
	   
	//NOTE(kai): if i'm doing an operation on an instance of this class
	//			,and the instance exist on the right hand side, then i must 
	//			use the overloaded operator function as a separate friend function (not part of the class)

	//Add 2 vectors using the '+' operator
	friend vec3 operator+(vec3 &left, vec3 &right);
	//Subtract 2 vectors using the '-' operator
	friend vec3 operator-(vec3 &left, vec3 &right);
	   		
	//Multiply 2 vectors using the '*' operator
	friend vec3 operator*(vec3 &left, vec3 &right);
	//Multiply vector with a constant value using the '*' operator
	friend vec3 operator*(vec3 &left, float &right);
	friend vec3 operator*(float &left, vec3 &right);
	   		
	//Divide 2 vectors using the '/' operator
	friend vec3 operator/(vec3 &left, vec3 &right);
	//Divide vector by a constant value using the '/' operator
	friend vec3 operator/(vec3 &left, float &right);
	friend vec3 operator/(float &left, vec3 &right);
	   
	//Check if 2 vectors are equal
	bool operator==(vec3 &other);
	//Check if 2 vectors are not equal
	bool operator!=(vec3 &other);
	   
	//Add 2 vectors using the '+=' operator
	vec3 operator+=(vec3 &other);
	//Subtract 2 vectors using the '-=' operator
	vec3 operator-=(vec3 &other);
	   
	//Multiply 2 vectors using the '*=' operator
	vec3 operator*=(vec3 &other);
	//Multiply vector with a constant value using the '*=' operator
	vec3 operator*=(float &other);
	   
	//Divide 2 vectors using the '/=' operator
	vec3 operator/=(vec3 &other);
	//Divide vector by a constant value using the '/=' operator
	vec3 operator/=(float &other);

public:
	//Vector data
	float x, y, z;
};

