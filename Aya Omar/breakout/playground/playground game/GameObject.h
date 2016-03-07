#pragma once
#include <GL/glew.h>
#include <Math\vec3.h>
#include <Math\vec2.h>


#include <Mesh.h>
#include<Shader.h>
#include <Transform.h>

struct ballobject {
	float raduis;
	bool stuck = true;

};

struct GameObject {

	Mesh  *Sprite;

	vec2    Size, velocity;
	vec3         Position;
	Color      * Color;
	Texture * Text;
	Transform *player_transform;
	GLfloat     Rotation = 0.0f;
	GLboolean   IsSolid = false;
	GLboolean   Destroyed = false;
	ballobject * ball_obj;
};

// void no return values 
void initGameObject(GameObject *object, Mesh *sprite, vec2 size, vec3 Position, Color* color,
	Texture *text);

Mesh*  createobjectSprite(GameObject *object);

void moveball(GameObject * ball, ballobject*ball_,Mesh *ballmesh, float screenwidth, float screenheight);
void checkballpos(GameObject * ball, Mesh *ballmesh, float screenwidth, float screenheight);


