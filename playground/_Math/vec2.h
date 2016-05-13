#pragma once

#include "MathFunctions.h"

struct vec2
{
public:
	vec2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}

	~vec2(){}
public:
	//Get vector length
	float Length();

	//Get vector length ^ 2
	float LengthSquare();

	//Normalize the vector
	vec2 Normalize();	

	//NOTE(kai): if i'm doing an operation on an instance of this class
	//			,and the instance exist on the right hand side, then i must 
	//			use the overloaded operator function as a separate friend function (not part of the class)

	//Add 2 vectors using the '+' operator
	friend vec2 operator+(vec2 &left, vec2 &right);
	//Subtract 2 vectors using the '-' operator
	friend vec2 operator-(vec2 &left, vec2 &right);

	//Multiply 2 vectors using the '*' operator
	friend vec2 operator*(vec2 &left, vec2 &right);
	//Multiply vector with a constant value using the '*' operator
	friend vec2 operator*(vec2 &left, float right);
	friend vec2 operator*(float &left, vec2 right);

	//Divide 2 vectors using the '/' operator
	friend vec2 operator/(vec2 &left, vec2 &right);
	//Divide vector by a constant value using the '/' operator
	friend vec2 operator/(vec2 &left, float right);
	friend vec2 operator/(float &left, vec2 right);

	//Check if 2 vectors are equal
	bool operator==(vec2 &other);
	//Check if 2 vectors are not equal
	bool operator!=(vec2 &other);

	//Add 2 vectors using the '+=' operator
	vec2 operator+=(vec2 &other);
	//Subtract 2 vectors using the '-=' operator
	vec2 operator-=(vec2 &other);

	//Multiply 2 vectors using the '*=' operator
	vec2 operator*=(vec2 &other);
	//Multiply vector with a constant value using the '*=' operator
	vec2 operator*=(float &other);

	//Divide 2 vectors using the '/=' operator
	vec2 operator/=(vec2 &other);
	//Divide vector by a constant value using the '/=' operator
	vec2 operator/=(float &other);

public:
	//Vector data
	float x, y;
};
