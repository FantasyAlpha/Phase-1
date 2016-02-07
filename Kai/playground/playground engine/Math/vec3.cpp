#include "vec3.h"

float vec3::Length()
{
	return (sqrtf((x * x) + (y * y) + (z * z)));
}

float vec3::LengthSquare()
{
	return ((x * x) + (y * y) + (z * z));
}

void vec3::Normalize()
{
	float length = this->Length();

	this->x /= length;
	this->y /= length;
	this->z /= length;
}

//Add 2 vectors using the '+' operator
vec3 operator+(vec3 &left, vec3 &right)
{
	vec3 result;

	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;

	return result;
}

//Subtract 2 vectors using the '-' operator
vec3 operator-(vec3 &left, vec3 &right)
{
	vec3 result;

	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;

	return result;
}

//Multiply 2 vectors using the '*' operator
vec3 operator*(vec3 &left, vec3 &right)
{
	vec3 result;

	result.x = left.x * right.x;
	result.y = left.y * right.y;
	result.z = left.z * right.z;

	return result;
}

//Multiply vector with a constant value using the '*' operator
vec3 operator*(vec3 &left, float &right)
{
	vec3 result;

	result.x = left.x * right;
	result.y = left.y * right;
	result.z = left.z * right;

	return result;
}

//Multiply vector with a constant value using the '*' operator
vec3 operator*(float &left, vec3 &right)
{
	vec3 result;

	result.x = left * right.x;
	result.y = left * right.y;
	result.z = left * right.z;

	return result;
}

//Divide 2 vectors using the '/' operator
vec3 operator/(vec3 &left, vec3 &right)
{
	vec3 result;

	result.x = left.x / right.x;
	result.y = left.y / right.y;
	result.z = left.z / right.z;

	return result;
}

//Divide 2 vectors using the '/' operator
vec3 operator/(vec3 &left, float &right)
{
	vec3 result;

	result.x = left.x / right;
	result.y = left.y / right;
	result.z = left.z / right;

	return result;
}

//Divide vector by a constant value using the '/' operator
vec3 operator/(float &left, vec3 &right)
{
	vec3 result;

	result.x = left / right.x;
	result.y = left / right.y;
	result.z = left / right.z;

	return result;
}

//Divide vector by a constant value using the '/' operator
vec3 vec3::operator+=(vec3 &other)
{
	vec3 result;

	result.x = this->x + other.x;
	result.y = this->y + other.y;
	result.z = this->z + other.z;

	return result;
}

//Subtract 2 vectors using the '-=' operator
vec3 vec3::operator-=(vec3 &other)
{
	vec3 result;

	result.x = this->x - other.x;
	result.y = this->y - other.y;
	result.z = this->z - other.z;

	return result;
}

//Multiply 2 vectors using the '*=' operator
vec3 vec3::operator*=(vec3 &other)
{
	vec3 result;

	result.x = this->x * other.x;
	result.y = this->y * other.y;
	result.z = this->z * other.z;

	return result;
}

//Multiply vector with a constant value using the '*=' operator
vec3 vec3::operator*=(float &other)
{
	vec3 result;

	result.x = this->x * other;
	result.y = this->y * other;
	result.z = this->z * other;

	return result;
}

//Divide 2 vectors using the '/=' operator
vec3 vec3::operator/=(vec3 &other)
{
	vec3 result;

	result.x = this->x / other.x;
	result.y = this->y / other.y;
	result.z = this->z / other.z;

	return result;
}

//Divide vector by a constant value using the '/=' operator
vec3 vec3::operator/=(float &other)
{
	vec3 result;

	result.x = this->x / other;
	result.y = this->y / other;
	result.z = this->z / other;

	return result;
}

//Check if 2 vectors are equal
bool vec3::operator==(vec3 &other)
{
	return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
}

//Check if 2 vectors are not equal
bool vec3::operator!=(vec3 &other)
{
	return (this->x != other.x) && (this->y != other.y) && (this->z != other.z);
}

float vec3::Dot(vec3 &other)
{
	return ((this->x * other.x) + (this->y * other.y) + (this->z * other.z));
}

vec3 vec3::Cross(vec3 &other)
{
	float newX = (this->y * other.z) - (this->z * other.y);
	float newY = (this->x * other.z) - (this->z * other.x);
	float newZ = (this->x * other.y) - (this->y * other.x);

	return vec3(newX, newY, newZ);
}
