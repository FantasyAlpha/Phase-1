/*=======================================================================================================================
NOTE(kai): This file can:

1) Save mesh data in a GPU buffer (using VBO, EBO, VAO)
2) Draw the mesh
=======================================================================================================================*/

#pragma once

#include <GL\glew.h>
#include <Vertex.h>
#include <Types.h>
#include <Math\vec3.h>
#include <Math\vec2.h>
#include <Math\MathFunctions.h>
#include <stdint.h>
#include <Utilities.h>

enum ATTRIBUTE_INDEX
{
	POSITION = 0,
	TEXTURE_COORDINATES = 1,
	NORMALS = 2,
};

struct Mesh
{
	bool WithNormals;
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Handle to a special type object that stores a reference to the vertex buffer ,index buffer
	//	and the layout specification to the vertex itself 
	unsigned int VAO;		//Vertex array object
#endif

	//The Buffers' handles i will use to store the mesh data in
	unsigned int VBO;		//Vertex buffer 
	unsigned int EBO;		//Element buffer


	unsigned int IndicesCount;
};

//Create the buffers and store the mesh data in them
Mesh CreateMesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool withNormals = true);

//Bind the buffers
file_internal void BindMesh(Mesh *mesh);

//Unbind the buffers
file_internal void UnbindMesh();

//Draw the mesh
void DrawMesh(Mesh *mesh);

////
Mesh CreateSprite(vec3 pos, vec2 size);

Mesh CreateCube(vec3 pos, vec3 size);