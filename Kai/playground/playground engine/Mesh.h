/*=======================================================================================================================
NOTE(kai): This file can:

1) Save mesh data in a GPU buffer (using VBO, EBO, VAO)
2) Draw the mesh
=======================================================================================================================*/

#pragma once

#include <GL\glew.h>
#include <iostream>
#include "Vertex.h"
#include "UsefulDefines.h"
#include "Utilities.h"

struct Mesh
{
	//NOTE(kai): vertices are the positions of the each vertex in the mesh
	//			,indices are the order of the vertices that i want to draw in

	//Handle to a special type object that stores a reference to the vertex buffer ,index buffer
	//	and the layout specification to the vertex itself 
	unsigned int VAO;		//Vertex array object

	//The Buffers' handles i will use to store the mesh data in
	unsigned int VBO;		//Vertex buffer 
	unsigned int EBO;		//Element buffer

	//The number of the vertices and indices in the mesh
	unsigned int VerticesCount, IndicesCount;

	unsigned int TextureHandle;
	unsigned long TextureWidth, TextureHeight;
};

//Create the buffers and store the mesh data in them
void LoadMesh(Mesh *mesh, Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);

//NOTE(kai): We create the vertex buffer by:
//					1) Create a handle for the buffer
//					2) Bind the buffer
//					3) Store the data in the buffer
//					4) Specify the vertex layout by enabling the attributes in the shader
//						,set the attribute pointer

//Create a Vertex buffer and stores the vertices in it 
file_internal void InitVBO(Mesh *mesh, Vertex *vertices);

//NOTE(kai): We create the element buffer by:
//					1) Create a handle for the buffer
//					2) Bind the buffer
//					3) Store the data in the buffer

//Create an Element buffer and stores the indices in it 
file_internal void InitEBO(Mesh *mesh, unsigned int *indices);

//NOTE(kai): only one buffer of the same type can be bound at a certain time

//Bind the buffers
file_internal void BindMesh(Mesh *mesh);

//Unbind the buffers
file_internal void UnbindMesh(Mesh *mesh);

//Draw the mesh
void DrawMesh(Mesh *mesh);

////
void SetTexture(Mesh *mesh, char *imagePath);
void BindTexture(Mesh *mesh);
void UnbindTexture(Mesh *mesh);

////
Mesh* CreateSprite(int width, int height, int pos, char *imagePath, Color color);