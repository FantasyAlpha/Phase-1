#pragma once

#include <GL\glew.h>
#include "Vertex.h"
#include "Types.h"
#include <stdint.h>
#include "Utilities.h"
#include "Texture.h"

struct AnimationClip;
struct GameObject;

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
	uint32 VBO;		
	uint32 EBO;		

	uint32 VerticesCount;
	uint32 IndicesCount;
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

	float TextureSlots[8];
	uint32 UsedSlotsCount;

	bool WithNormals;
	bool Stop;
};

struct Sprite
{
	vec3f Pos;
	vec3f Size;
	Material Skin;
	GameObject* Owner; 
	Mesh Buffers;
};

//Create the buffers and store the mesh data in them
Mesh CreateMesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount, bool withNormals = true, bool batch = false);

//Bind the buffers
void BindMesh(Mesh *mesh);

//Unbind the buffers
void UnbindMesh();

void DrawMesh(Mesh *mesh, bool debug);

////
Mesh CreateSprite(vec3f pos, vec3f size, vec4f color, bool withNormals, bool debug);

void EditSprite(Mesh *sprite, vec3f pos, vec3f size, vec4f color);

Mesh CreateCube(vec3f pos, vec3f size, vec4f color, bool withNormals = true);

void CalculateNormals(Vertex *vertices, uint32 vertCount, unsigned int *indices, unsigned int indicesCount);
void EditNormals(Mesh *buffers);

void BeginBatch(MeshBatch *batch, BATCH_TYPE type, uint32 maxCount, bool debug);

void PauseBatch(MeshBatch *batch);

void ResumeBatch(MeshBatch *batch);

void AddSprite(MeshBatch *batch, vec3f pos, vec3f size, vec4f color, uint32 textureID, bool debug = false, mat4f model = mat4f());

void EndBatch(MeshBatch *batch, bool debug);

void RenderBatch(MeshBatch *batch, bool debug);

void DrawSprite(Sprite *sprite);