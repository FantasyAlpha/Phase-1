/*=======================================================================================================================
NOTE(kai): This file contain:

1) Vertex data structure
2) Position (x, y, z) data structure
3) Color (R, G, B) data structure
=======================================================================================================================*/

#pragma once

struct Position
{
	Position(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float x;
	float y;
	float z;
};

struct Color
{
	Color(float r = 0, float g = 0, float b = 0, float a = 0)
	{
		this->red = r;
		this->green = g;
		this->blue = b;
		this->alpha = a;		
	}

	float red;
	float green;
	float blue;
	float alpha;
};

struct TexCoords
{
	TexCoords(float S = 0, float T = 0)
	{
		this->S = S;
		this->T = T;
	}

	float S;
	float T;
};

struct Vertex
{
	Vertex(Position pos = Position(0.0f, 0.0f, 0.0f), Color color = Color(0, 0, 0, 0), TexCoords texCoords = (0, 0))
	{
		this->pos = pos;
		this->color = color;
		this->texCoords = texCoords;
	}

	Position pos;
	Color color;
	TexCoords texCoords;
};