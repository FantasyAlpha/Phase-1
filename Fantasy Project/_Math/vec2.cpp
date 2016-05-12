#include "Math\vec2.h"

float vec2::Length()
{
	return (sqrtf((x * x) + (y * y)));
}

float vec2::LengthSquare()
{
	return ((x * x) + (y * y));
}

vec2 vec2::Normalize()
{
	float length = this->Length();
	vec2 result;

	if (length == 0.0f)
	{
		return *this;
	}
	else
	{
		result.x = x / length;
		result.y = y / length;

		return result;
	}
}

//Add 2 vectors using the '+' operator
vec2 operator+(vec2 &left, vec2 &right)
{
	vec2 result;

	result.x = left.x + right.x;
	result.y = left.y + right.y;

	return result;
}

//Subtract 2 vectors using the '-' operator
vec2 operator-(vec2 &left, vec2 &right)
{
	vec2 result;

	result.x = left.x - right.x;
	result.y = left.y - right.y;

	return result;
}

//Multiply 2 vectors using the '*' operator
vec2 operator*(vec2 &left, vec2 &right)
{
	vec2 result;

	result.x = left.x * right.x;
	result.y = left.y * right.y;

	return result;
}

//Multiply vector with a constant value using the '*' operator
vec2 operator*(vec2 &left, float &right)
{
	vec2 result;

	result.x = left.x * right;
	result.y = left.y * right;

	return result;
}

//Multiply vector with a constant value using the '*' operator
vec2 operator*(float &left, vec2 &right)
{
	vec2 result;

	result.x = left * right.x;
	result.y = left * right.y;

	return result;
}

//Divide 2 vectors using the '/' operator
vec2 operator/(vec2 &left, vec2 &right)
{
	vec2 result;

	result.x = left.x / right.x;
	result.y = left.y / right.y;

	return result;
}

//Divide 2 vectors using the '/' operator
vec2 operator/(vec2 &left, float &right)
{
	vec2 result;

	result.x = left.x / right;
	result.y = left.y / right;

	return result;
}

//Divide vector by a constant value using the '/' operator
vec2 operator/(float &left, vec2 &right)
{
	vec2 result;

	result.x = left / right.x;
	result.y = left / right.y;

	return result;
}

//Divide vector by a constant value using the '/' operator
vec2 vec2::operator+=(vec2 &other)
{
	vec2 result;

	result.x = this->x + other.x;
	result.y = this->y + other.y;

	return result;
}

//Subtract 2 vectors using the '-=' operator
vec2 vec2::operator-=(vec2 &other)
{
	vec2 result;

	result.x = this->x - other.x;
	result.y = this->y - other.y;

	return result;
}

//Multiply 2 vectors using the '*=' operator
vec2 vec2::operator*=(vec2 &other)
{
	vec2 result;

	result.x = this->x * other.x;
	result.y = this->y * other.y;

	return result;
}

//Multiply vector with a constant value using the '*=' operator
vec2 vec2::operator*=(float &other)
{
	vec2 result;

	result.x = this->x * other;
	result.y = this->y * other;

	return result;
}

//Divide 2 vectors using the '/=' operator
vec2 vec2::operator/=(vec2 &other)
{
	vec2 result;

	result.x = this->x / other.x;
	result.y = this->y / other.y;

	return result;
}

//Divide vector by a constant value using the '/=' operator
vec2 vec2::operator/=(float &other)
{
	vec2 result;

	result.x = this->x / other;
	result.y = this->y / other;

	return result;
}

//Check if 2 vectors are equal
bool vec2::operator==(vec2 &other)
{
	return (this->x == other.x) && (this->y == other.y);
}

//Check if 2 vectors are not equal
bool vec2::operator!=(vec2 &other)
{
	return (this->x != other.x) && (this->y != other.y);
}