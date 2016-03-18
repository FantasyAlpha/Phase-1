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
#include <stdint.h>

enum MESH_TYPE
{
	NORMAL_MESH = 0,
	DYNAMIC_SPRITE_BATCH = 1,
	STATIC_SPRITE_BATCH = 2
};

struct MeshBuffers
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Handle to a special type object that stores a reference to the vertex buffer ,index buffer
	//	and the layout specification to the vertex itself 
	unsigned int VAO;		//Vertex array object
#endif

	//The Buffers' handles i will use to store the mesh data in
	unsigned int VBO;		//Vertex buffer 
	unsigned int EBO;		//Element buffer	

	//The number of the indices in the mesh
	unsigned int IndicesCount;
	MESH_TYPE MeshType;
	unsigned int MAX_SIZE;
};
struct Collider
{
	vec2 size;
	vec3 pos;
	bool detected;
	float penetration;
	vec2 normal;
};

struct Mesh
{
	MeshBuffers Buffers;
	Texture MeshTexture;
	Transform GlobalTransform;
	Collider   collider;
};

struct SpriteBatch
{
	MeshBuffers Buffers;

	void *DataBuffer;

	Vertex *VerticesBuffer;
	unsigned int *IndicesBuffer;

	Texture *MeshTexture;

	int BatchSize;
	int UsedSize;
};

//Create the buffers and store the mesh data in them
void LoadMesh(MeshBuffers *buffers, Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);

//Bind the buffers
file_internal void BindMesh(MeshBuffers *buffers);

//Unbind the buffers
file_internal void UnbindMesh();

//Draw the mesh
void DrawMesh(Mesh *mesh);

////
Mesh CreateSprite(vec2 size, vec3 pos, Texture *texture, Color *colors, int colorCount);

void AddSpriteToBatch(SpriteBatch *batch, int spriteCount, vec3 *pos, int posCount, vec2 *size, int sizeCount, Texture *texture, int textureCount, Color *colors, int colorCount);

void InitSpriteBatch(SpriteBatch *batch, unsigned int maxSize, MESH_TYPE type);

void BeginStoringInSpriteBatch(SpriteBatch *batch);
void AllocateStaticSpriteBatch(SpriteBatch *batch, int size);
void EndStoringInSpriteBatch(SpriteBatch *batch);

void AllocateDynamicSpriteBatch(SpriteBatch *batch, int size);

void RenderSpriteBatch(SpriteBatch *batch);

void UpdateSpriteBatch(SpriteBatch *batch, Transform *transform, int transformCount, int *spriteIndex, int spriteCount);