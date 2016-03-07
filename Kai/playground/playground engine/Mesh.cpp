#include "Mesh.h"

static bool Glew_Initialized = false;

//Create the buffers and store the mesh data in them
void LoadMesh(MeshBuffers *buffers, Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
{
	if (!Glew_Initialized){
		bool res = ReloadGlew();
		Glew_Initialized = true;
	}
	buffers->IndicesCount = indicesCount;

	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	if (!buffers->VAO)
	{
		//Generate a handle to a vertex array object
		glGenVertexArrays(1, &buffers->VAO);
	}	
#endif

	if (!buffers->VBO)
	{
		//Generate a handle to a vertex buffer object
		glGenBuffers(1, &buffers->VBO);
	}

	if (!buffers->EBO)
	{
		//Generate a handle to a element buffer object
		glGenBuffers(1, &buffers->EBO);
	}

#if GLSL_VERSION == MODERN_VERSION
	//Generate a handle to a vertex array object
	glBindVertexArray(buffers->VAO);
#endif

	/***********************************************************************************************************/
	//Create a Vertex buffer
	//NOTE(kai): We create the vertex buffer by:
	//					1) Create a handle for the buffer
	//					2) Bind the buffer
	//					3) Store the data in the buffer
	//					4) Specify the vertex layout by enabling the attributes in the shader
	//						,set the attribute pointer

	/***********************************************************************************************************/
	
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);

	//Store the data in the buffer
	if (buffers->MeshType == MESH_TYPE::STATIC_SPRITE_BATCH)
	{
		glBufferData(GL_ARRAY_BUFFER, buffers->MAX_SIZE * 4 * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);	
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesCount * sizeof(Vertex), vertices);
	}

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(0				//The attribute index
		, 3				//Number of elements
		, GL_FLOAT		//Type of elements
		, GL_FALSE		//Normalized or not
		, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
		, 0);			//The pointer offset

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(vec3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(vec3) + sizeof(Color)));

	/***********************************************************************************************************/
	/***********************************************************************************************************/

	/***********************************************************************************************************/
	//Create an Element buffer
	//NOTE(kai): We create the element buffer by:
	//					1) Create a handle for the buffer
	//					2) Bind the buffer
	//					3) Store the data in the buffer

	/***********************************************************************************************************/
	
	//Bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);

	GLenum err = glGetError();
	//Store the data in the buffer
	if (buffers->MeshType == MESH_TYPE::STATIC_SPRITE_BATCH)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffers->MAX_SIZE * 6 * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);	
		err = glGetError();
	}
	else
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesCount * sizeof(unsigned int), indices);
	}

	/***********************************************************************************************************/
	/***********************************************************************************************************/

	//Unbind everything
	UnbindMesh();
}

//Bind the buffers
file_internal void BindMesh(MeshBuffers *buffers)
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Bind the element buffer
	glBindVertexArray(buffers->VAO);

#elif GLSL_VERSION == ANCIENT_VERSION	//Use with older GLSL versions
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers->VBO);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(0				//The attribute index
		, 3				//Number of elements
		, GL_FLOAT		//Type of elements
		, GL_FALSE		//Normalized or not
		, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
		, 0);			//The pointer offset

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(vec3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(vec3) + sizeof(Color)));

	//Bind the vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers->EBO);
#endif
}

//Unbind the buffers
file_internal void UnbindMesh()
{
	//Use only with newer GLSL versions
#if GLSL_VERSION == MODERN_VERSION
	//Unbind the vertex array object
	glBindVertexArray(0);
#endif
	//Unbind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//Unbind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Draw the mesh
void DrawMesh(Mesh *mesh)
{
	//Bind the buffers
	BindMesh(&mesh->Buffers);

	BindTexture(&mesh->MeshTexture);

	//Draw the mesh
	glDrawElements(GL_TRIANGLES		//Drawing type
				 , mesh->Buffers.IndicesCount	//Number of indices
				 , GL_UNSIGNED_INT	//Indices type
				 , 0);				//The location of the indices (NULL means to look for them in the buffer) 

	//Unbind the buffers
	UnbindMesh();
	UnbindTexture();
}

////
void CreateSprite(Mesh *sprite, vec2 size, vec3 pos, Texture *texture, Color *colors, int colorCount)
{
	//Square vertices 
	Vertex vertices[] =
	{
		Vertex(vec3(pos.x			, pos.y			  , pos.z), colorCount == 1 ? *colors : colors[0], vec2(1, 1)),		//TOP RIGHT
		Vertex(vec3(pos.x			, pos.y + (size.y), pos.z), colorCount == 1 ? *colors : colors[1], vec2(1, 0)),		//BOTTOM RIGHT
		Vertex(vec3(pos.x + (size.x), pos.y + (size.y), pos.z), colorCount == 1 ? *colors : colors[2], vec2(0, 0)),		//BOTTOM LEFT
		Vertex(vec3(pos.x + (size.x), pos.y			  , pos.z), colorCount == 1 ? *colors : colors[3], vec2(0, 1)),		//TOP LEFT
	};


	//Order of vertices that will be drawn
	unsigned int indices[] =
	{
		0,
		1,
		3,
		1,
		2,
		3,
	};

	LoadMesh(&sprite->Buffers, vertices, sizeof(vertices) / sizeof(Vertex), indices, sizeof(indices) / sizeof(unsigned int));
	sprite->MeshTexture = *texture;
}

void InitSpriteBatch(SpriteBatch *batch, unsigned int maxSize, MESH_TYPE type)
{
	batch->Buffers.MAX_SIZE = maxSize;	
	batch->Buffers.MeshType = type;

	if (maxSize > 0 && type == MESH_TYPE::DYNAMIC_SPRITE_BATCH)
	{
		unsigned int newSize = (batch->UsedSize + maxSize) * 2;
		int newBufferSize = ((sizeof(Vertex) * 4) + (sizeof(unsigned int) * 6)) * newSize;

		void *newDataBuffer = malloc(newBufferSize);

		Vertex *newVertices = (Vertex *)newDataBuffer;
		unsigned int *newIndices = (unsigned int *)(newVertices + (newSize * 4));
		batch->UsedSize = 0;
		
		free(batch->DataBuffer);

		batch->DataBuffer = newDataBuffer;
		batch->IndicesBuffer = newIndices;
		batch->VerticesBuffer = newVertices;
		batch->BatchSize = newSize;
	}
	else if (type == MESH_TYPE::STATIC_SPRITE_BATCH)
	{
		LoadMesh(&batch->Buffers, batch->VerticesBuffer, batch->UsedSize * 4, batch->IndicesBuffer, batch->UsedSize * 6);
	}
}

void AddSpriteToBatch(SpriteBatch *batch, int spriteCount, vec3 *pos, int posCount, vec2 *size, int sizeCount, Texture *texture, int textureCount, Color *colors, int colorCount)
{
	if (spriteCount > 0)
	{
		int verticesIndex = 0;
		int indicesIndex = 0;

		if (batch->Buffers.MeshType == MESH_TYPE::STATIC_SPRITE_BATCH && batch->UsedSize + spriteCount > batch->Buffers.MAX_SIZE)
		{
			spriteCount = 0;
		}

		if (batch->Buffers.MeshType == MESH_TYPE::STATIC_SPRITE_BATCH)
		{
			AllocateStaticSpriteBatch(batch, spriteCount);
		}
		else
		{
			AllocateDynamicSpriteBatch(batch, spriteCount);
			verticesIndex = (batch->UsedSize - spriteCount) * 4;
			indicesIndex = (batch->UsedSize - spriteCount) * 6;
		}		

		int offset = (batch->UsedSize - spriteCount);
		int posIndex = 0;
		int sizeIndex = 0;

		for (int i = 0; i < spriteCount; i++)
		{
			if (posIndex == posCount)
			{
				posIndex = 0;
			}

			if (sizeIndex == sizeCount)
			{
				sizeIndex = 0;
			}

			//Square vertices 
			*(batch->VerticesBuffer + verticesIndex++) = Vertex(vec3((pos + posIndex)->x, (pos + posIndex)->y, (pos + posIndex)->z), colorCount == 1 ? *colors : colors[0], vec2(1, 1));	//TOP RIGHT
			*(batch->VerticesBuffer + verticesIndex++) = Vertex(vec3((pos + posIndex)->x, (pos + posIndex)->y + ((size + sizeIndex)->y), (pos + posIndex)->z), colorCount == 1 ? *colors : colors[1], vec2(1, 0));	//BOTTOM RIGHT
			*(batch->VerticesBuffer + verticesIndex++) = Vertex(vec3((pos + posIndex)->x + ((size + sizeIndex)->x), (pos + posIndex)->y + ((size + sizeIndex)->y), (pos + posIndex)->z), colorCount == 1 ? *colors : colors[2], vec2(0, 0));	//BOTTOM LEFT
			*(batch->VerticesBuffer + verticesIndex++) = Vertex(vec3((pos + posIndex)->x + ((size + sizeIndex)->x), (pos + posIndex)->y, (pos + posIndex)->z), colorCount == 1 ? *colors : colors[3], vec2(0, 1));	//TOP LEFT

			posIndex++;
			sizeIndex++;

			//Order of vertices that will be drawn		 
			*(batch->IndicesBuffer + indicesIndex++) = 0 + (offset * 4);
			*(batch->IndicesBuffer + indicesIndex++) = 1 + (offset * 4);
			*(batch->IndicesBuffer + indicesIndex++) = 3 + (offset * 4);
			*(batch->IndicesBuffer + indicesIndex++) = 1 + (offset * 4);
			*(batch->IndicesBuffer + indicesIndex++) = 2 + (offset * 4);
			*(batch->IndicesBuffer + indicesIndex++) = 3 + (offset * 4);
			offset++;
		}	

		if (batch->Buffers.MeshType == MESH_TYPE::STATIC_SPRITE_BATCH)
		{
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}

		batch->MeshTexture = texture;
	}	
}
void BeginStoringInSpriteBatch(SpriteBatch *batch)
{
	//Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, batch->Buffers.VBO);

	//Enable the attribute with the given index in the shader to be used in rendering
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Defines where the attribute with the given index should look in buffer
	glVertexAttribPointer(0				//The attribute index
		, 3				//Number of elements
		, GL_FLOAT		//Type of elements
		, GL_FALSE		//Normalized or not
		, sizeof(Vertex)//Size of the stride (the next location that the pointer will go to)
		, 0);			//The pointer offset

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(vec3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(vec3) + sizeof(Color)));

	//Bind the vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->Buffers.EBO);
}

void EndStoringInSpriteBatch(SpriteBatch *batch)
{
	if (batch->Buffers.MeshType == MESH_TYPE::DYNAMIC_SPRITE_BATCH)
	{
		LoadMesh(&batch->Buffers, batch->VerticesBuffer, batch->UsedSize * 4, batch->IndicesBuffer, batch->UsedSize * 6);
	}
	

	//Unbind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AllocateStaticSpriteBatch(SpriteBatch *batch, int size)
{	
	if (size > 0)
	{
		batch->VerticesBuffer = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		batch->VerticesBuffer += (batch->UsedSize * 4);
		batch->IndicesBuffer = (unsigned int *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
		batch->IndicesBuffer += (batch->UsedSize * 6);
		batch->BatchSize += size;
		batch->UsedSize += size;
	}
}

void AllocateDynamicSpriteBatch(SpriteBatch *batch, int size)
{
	unsigned int newBufferSize = 0;
	if (batch->UsedSize + size >= batch->BatchSize)
	{
		unsigned int newSize = (batch->UsedSize + size) * 2;
		newBufferSize = ((sizeof(Vertex) * 4) + (sizeof(unsigned int) * 6)) * newSize;
		
		void *newDataBuffer = malloc(newBufferSize);

		Vertex *newVertices = (Vertex *)newDataBuffer;
		unsigned int *newIndices = (unsigned int *)(newVertices + (newSize * 4));

		memcpy_s(newVertices, newSize * sizeof(Vertex) * 4, batch->VerticesBuffer, ((batch->UsedSize) * sizeof(Vertex) * 4));
		memcpy_s(newIndices, newSize * sizeof(unsigned int) * 6, batch->IndicesBuffer, ((batch->UsedSize) * sizeof(unsigned int) * 6));

		free(batch->DataBuffer);

		batch->DataBuffer = newDataBuffer;
		batch->IndicesBuffer = newIndices;
		batch->VerticesBuffer = newVertices;
		batch->BatchSize = newSize;
	}
	batch->UsedSize += size;
}



void RenderSpriteBatch(SpriteBatch *batch)
{
	//Bind the buffers
	BindMesh(&batch->Buffers);

	BindTexture(batch->MeshTexture);

	//Draw the mesh
	glDrawElements(GL_TRIANGLES		//Drawing type
				 , batch->UsedSize * 6	//Number of indices
				 , GL_UNSIGNED_INT	//Indices type
				 , 0);				//The location of the indices (NULL means to look for them in the buffer) 

	//Unbind the buffers
	UnbindMesh();
	UnbindTexture();
}

void UpdateSpriteBatch(SpriteBatch *batch, Transform *transform, int transformCount, int *spriteIndex, int spriteCount)
{	
	if (batch->Buffers.MeshType != MESH_TYPE::STATIC_SPRITE_BATCH && batch->UsedSize > 0)
	{
		int transIndex = 0;
		Transform t = *transform; 

		for (int i = 0; i < spriteCount; i++)
		{
			if (transIndex >= transformCount)
			{
				transIndex = transformCount;
			}

			batch->VerticesBuffer[(spriteIndex[i] * 4) + 0].pos = batch->VerticesBuffer[(spriteIndex[i] * 4) + 0].pos * transform->ModelMatrix;
			batch->VerticesBuffer[(spriteIndex[i] * 4) + 1].pos = batch->VerticesBuffer[(spriteIndex[i] * 4) + 1].pos * transform->ModelMatrix;
			batch->VerticesBuffer[(spriteIndex[i] * 4) + 2].pos = batch->VerticesBuffer[(spriteIndex[i] * 4) + 2].pos * transform->ModelMatrix;
			batch->VerticesBuffer[(spriteIndex[i] * 4) + 3].pos = batch->VerticesBuffer[(spriteIndex[i] * 4) + 3].pos * transform->ModelMatrix;

			transIndex++;
		}
		
		transform->Position = vec3();
		transform->Rotation = vec3();
		transform->Scale = vec3(1, 1, 1);
		transform->ModelMatrix = mat4(1.0f);

		LoadMesh(&batch->Buffers, batch->VerticesBuffer, batch->UsedSize * 4, batch->IndicesBuffer, batch->UsedSize * 6);
	}	
}