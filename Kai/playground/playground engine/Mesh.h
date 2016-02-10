/*=======================================================================================================================
NOTE(kai): This file can:

1) Save mesh data in a GPU buffer (using VBO, EBO, VAO)
2) Draw the mesh
=======================================================================================================================*/

#pragma once

#include <GL\glew.h>
#include "Vertex.h"
#include "Texture.h"
#include "Math\vec3.h"
#include "Math\vec2.h"
#include "Transform.h"

struct MeshBuffers
{
	//Handle to a special type object that stores a reference to the vertex buffer ,index buffer
	//	and the layout specification to the vertex itself 
	unsigned int VAO;		//Vertex array object

	//The Buffers' handles i will use to store the mesh data in
	unsigned int VBO;		//Vertex buffer 
	unsigned int EBO;		//Element buffer
};

struct MeshData
{
	//NOTE(kai): vertices are the positions of the each vertex in the mesh
	//			,indices are the order of the vertices that i want to draw in

	Vertex *Vertices;
	unsigned int *Indices;

	//The number of the vertices and indices in the mesh
	unsigned int VerticesCount, IndicesCount;
};

struct Sprite
{
	vec2 Size;

	MeshData Data;
	MeshBuffers Buffers;

	Texture SpriteTexture;
	Transform GlobalTransform;
};

struct Mesh
{
	MeshData Data;
	MeshBuffers Buffers;

	Texture MeshTexture;
	Transform GlobalTransform;
};

//Create the buffers and store the mesh data in them
void LoadMesh(MeshBuffers *buffers, MeshData *data, Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);

//NOTE(kai): We create the vertex buffer by:
//					1) Create a handle for the buffer
//					2) Bind the buffer
//					3) Store the data in the buffer
//					4) Specify the vertex layout by enabling the attributes in the shader
//						,set the attribute pointer

//Create a Vertex buffer and stores the vertices in it 
file_internal void InitVBO(MeshBuffers *buffers, MeshData *data);

//NOTE(kai): We create the element buffer by:
//					1) Create a handle for the buffer
//					2) Bind the buffer
//					3) Store the data in the buffer

//Create an Element buffer and stores the indices in it 
file_internal void InitEBO(MeshBuffers *buffers, MeshData *data);

//NOTE(kai): only one buffer of the same type can be bound at a certain time

//Bind the buffers
file_internal void BindMesh(MeshBuffers *buffers);

//Unbind the buffers
file_internal void UnbindMesh();

//Draw the mesh
void DrawMesh(Mesh *mesh);

////
void DrawSprite(Sprite *sprite);
void CreateSprite(Sprite *sprite, vec2 size, vec3 pos, Texture *texture, Color *colors, int colorCount);