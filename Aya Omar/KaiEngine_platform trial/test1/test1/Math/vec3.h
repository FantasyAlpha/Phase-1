#pragma once
#include "vec2.h"

struct vec3
{
public:
	vec3(void){}
	vec3(float x, float y, float z){}
	~vec3(){}
public:
	//Get vector length
	float getLength();

	//Get vector length ^ 2
	float getLengthSquare();

	//Normalize the vector
	void normalize();

	//Add 2 vectors
	vec3& add(vec3 &other);
	//Subtract 2 vectors
	vec3& sub(vec3 &other);
	 
	//Multiply 2 vectors
	vec3& mul(vec3 &other);
	//Multiply vector with a constant value
	vec3& mul(float other);
	   
	//Divide 2 vectors
	vec3& div(vec3 &other);
	//Divide vector by a constant value
	vec3& div(float other);
	   
	//NOTE(kai): if i'm doing an operation on an instance of this class
	//			,and the instance exist on the right hand side, then i must 
	//			use the overloaded operator function as a separate friend function (not part of the class)

	//Add 2 vectors using the '+' operator
	friend vec3 operator+(vec3 left, vec3& right);
	//Subtract 2 vectors using the '-' operator
	friend vec3 operator-(vec3 left, vec3& right);
	   		
	//Multiply 2 vectors using the '*' operator
	friend vec3 operator*(vec3 left, vec3& right);
	//Multiply vector with a constant value using the '*' operator
	friend vec3 operator*(vec3 left, float right);
	   		
	//Divide 2 vectors using the '/' operator
	friend vec3 operator/(vec3 left, vec3& right);
	//Divide vector by a constant value using the '/' operator
	friend vec3 operator/(vec3 left, float right);
	   
	//Check if 2 vectors are equal
	bool operator==(vec3& other);
	//Check if 2 vectors are not equal
	bool operator!=(vec3& other);
	   
	//Add 2 vectors using the '+=' operator
	vec3& operator+=(vec3& other);
	//Subtract 2 vectors using the '-=' operator
	vec3& operator-=(vec3& other);
	   
	//Multiply 2 vectors using the '*=' operator
	vec3& operator*=(vec3& other);
	//Multiply vector with a constant value using the '*=' operator
	vec3& operator*=(float other);
	   
	//Divide 2 vectors using the '/=' operator
	vec3& operator/=(vec3& other);
	//Divide vector by a constant value using the '/=' operator
	vec3& operator/=(float other);

public:
	//Vector data
	float x, y, z;
};

