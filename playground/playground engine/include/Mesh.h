#pragma once

#include <GL\glew.h>
#include <Vertex.h>
#include <Types.h>
#include <stdint.h>
#include <Utilities.h>

struct AnimationClip;

enum ATTRIBUTE_INDEX
{
	POSITION = 0,
	TEXTURE_COORDINATES = 1,
	COLOR = 2,
	TEXTURE_SLOT = 3,
	NORMALS = 4,
};

struct Mesh
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Handle to a special type object that stores a reference to the vertex buffer ,index buffer
	//	and the layout specification to the vertex itself 
	unsigned int VAO;		//Vertex array object
#endif

	//The Buffers' handles i will use to store the mesh data in
	uint32 VBO;		//Vertex buffer 
	uint32 EBO;		//Element buffer

	uint32 IndicesCount;

	bool WithNormals;
};

enum BATCH_TYPE
{
	UNKNOWN = 0,
	SPRITE_BATCH = 1,
	CUBE_BATCH = 2
};

struct MeshBatch
{
	Mesh Batch;

	uint32 MaxCount;
	uint32 CurrentSize;

	BATCH_TYPE Type;

	float TextureSlots[32];
	uint32 UsedSlotsCount;

	bool WithNormals;
};


//Create the buffers and store the mesh data in them
Mesh CreateMesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool withNormals = true, bool batch = false);

//Bind the buffers
void BindMesh(Mesh *mesh);

//Unbind the buffers
void UnbindMesh();

//Draw the mesh
void DrawMesh(Mesh *mesh);

////
Mesh CreateSprite(vec3f pos, vec2f size, vec4f color, bool withNormals = true);

Mesh CreateCube(vec3f pos, vec3f size, vec4f color, bool withNormals = true);

void CalculateNormals(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);

void BeginBatch(MeshBatch *batch, BATCH_TYPE type, uint32 maxCount, bool debug);

void PauseBatch(MeshBatch *batch);

void ResumeBatch(MeshBatch *batch);

void AddSprite(MeshBatch *batch, mat4f &modelMat, vec3f pos, vec2f size, vec4f color, uint32 textureID, AnimationClip *clip = 0, bool debug = false, bool withNormals = true);

void EndBatch(MeshBatch *batch, bool debug);

void RenderBatch(MeshBatch *batch, bool debug);