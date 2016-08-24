#include "Mesh.h"
#include <stdint.h>

Mesh CreateMesh(Vertex *vertices, unsigned int verticesCount
	, unsigned int *indices, unsigned int indicesCount, bool withNormals, bool batch)
{
	Mesh mesh = {};

	mesh.IndicesCount = indicesCount;
	mesh.VerticesCount = verticesCount;

	if (withNormals)
	{
		CalculateNormals(vertices, verticesCount, indices, indicesCount);
	}

	glGenBuffers(1, &mesh.VBO);
	glGenBuffers(1, &mesh.EBO);

	BindMesh(&mesh);


	if (!batch)
	{
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
	}

	UnbindMesh();

	return mesh;
}

void BindMesh(Mesh *mesh)
{
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(0			
		, 3				
		, GL_FLOAT		
		, GL_FALSE		
		, sizeof(Vertex)
		, (void *)offsetof(Vertex, Vertex::Pos));		

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertex::TexCoords));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertex::Color));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertex::TextureSlot));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, Vertex::Normal)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
}

void UnbindMesh()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DrawMesh(Mesh *mesh, bool debug)
{
	if (debug)
	{
		glLineWidth(1.0f);
		BindMesh(mesh);
		glDrawElements(GL_LINES, mesh->IndicesCount, GL_UNSIGNED_INT, NULL);
		UnbindMesh();
	}
	else
	{
		BindMesh(mesh);

		glDrawElements(GL_TRIANGLES		
			, mesh->IndicesCount	
			, GL_UNSIGNED_INT	
			, 0);			

		UnbindMesh();
	}
}

Mesh CreateSprite(vec3f pos, vec3f size, vec4f color, bool withNormals, bool debug)
{
	Mesh sprite = {};

	{
		vec3f lowerLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
		vec3f upperLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);
		vec3f lowerRight = vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
		vec3f upperRight = vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);

		Vertex vertices[] =
		{
			Vertex{ lowerLeft, vec2f(0, 0), color, 0 },
			Vertex{ upperLeft, vec2f(0, 1), color, 0 },
			Vertex{ upperRight, vec2f(1, 1), color, 0 },
			Vertex{ lowerRight, vec2f(1, 0), color, 0 }
		};

		if (debug)
		{
			uint32 indices[] =
			{
				0,
				1,

				1,
				2,

				2,
				3,

				3,
				0,
			};

			sprite = CreateMesh(vertices, 4, indices, 8, withNormals, 0);
		}
		else
		{
			unsigned int indices[] =
			{
				0, 1, 3,
				1, 2, 3,
			};

			sprite = CreateMesh(vertices, 4, indices, 6, withNormals, 0);
		}
	}	

	return sprite;
}

void EditSprite(Mesh *sprite, vec3f pos, vec3f size, vec4f color)
{
	BindMesh(sprite);

	Vertex *vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

	vec3f lowerLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
	vec3f upperLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);
	vec3f lowerRight = vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
	vec3f upperRight = vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);

	vertices[0].Pos = lowerLeft; 
	vertices[0].Color = color;

	vertices[1].Pos = upperLeft;
	vertices[1].Color = color;

	vertices[2].Pos = upperRight;
	vertices[2].Color = color;

	vertices[3].Pos = lowerRight;
	vertices[3].Color = color;

	glUnmapBuffer(GL_ARRAY_BUFFER);
	UnbindMesh();
}

Mesh CreateCube(vec3f pos, vec3f size, vec4f color, bool withNormals)
{
	Vertex vertices[24] =
	{
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(0, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 1), color },
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(0, 1), color },

		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(0, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(1, 1), color },
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 1), color },

		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(1, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(1, 1), color },
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 1), color },

		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(0, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(1, 1), color },
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 1), color },

		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 0), color },
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 0), color },
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 1), color },
		Vertex{ vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 1), color },

		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 0), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z + (size.Z / 2.0f)), vec2f(1, 1), color },
		Vertex{ vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z - (size.Z / 2.0f)), vec2f(0, 1), color },
	};

	uint32 indices[36] =
	{
		0, 1, 3,
		1, 2, 3,

		0 + 4, 1 + 4, 3 + 4,
		1 + 4, 2 + 4, 3 + 4,

		0 + 8, 1 + 8, 3 + 8,
		1 + 8, 2 + 8, 3 + 8,

		0 + 12, 1 + 12, 3 + 12,
		1 + 12, 2 + 12, 3 + 12,

		0 + 16, 1 + 16, 3 + 16,
		1 + 16, 2 + 16, 3 + 16,

		0 + 20, 1 + 20, 3 + 20,
		1 + 20, 2 + 20, 3 + 20,
	};

	Mesh result = CreateMesh(vertices, 24, indices, 36, withNormals);

	return result;
}

void CalculateNormals(Vertex *vertices, uint32 vertCount, unsigned int *indices, unsigned int indicesCount)
{
	for (uint32 i = 0; i < indicesCount; i += 3)
	{
		uint32 i0 = indices[i];
		uint32 i1 = indices[i + 1];
		uint32 i2 = indices[i + 2];

		vec3f v1 = vertices[i1].Pos - vertices[i0].Pos;
		vec3f v2 = vertices[i2].Pos - vertices[i0].Pos;

		vec3f normal = Vec3::Cross(v1, v2);
		normal = Vec3::Normalize(normal);

		vertices[i0].Normal += normal;
		vertices[i1].Normal += normal;
		vertices[i2].Normal += normal;
	}

	for (uint32 i = 0; i < vertCount; i++)
	{
		vertices[i].Normal = Vec3::Normalize(vertices[i].Normal);
	}
}

void EditNormals(Mesh *buffers)
{
	BindMesh(buffers);
	Vertex *vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	uint32 *indices = (uint32 *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);

	for (uint32 i = 0; i < buffers->IndicesCount; i += 3)
	{
		uint32 i0 = indices[i];
		uint32 i1 = indices[i + 1];
		uint32 i2 = indices[i + 2];

		vec3f v1 = vertices[i1].Pos - vertices[i0].Pos;
		vec3f v2 = vertices[i2].Pos - vertices[i0].Pos;

		vec3f normal = Vec3::Cross(v1, v2);
		normal = Vec3::Normalize(normal);

		vertices[i0].Normal += normal;
		vertices[i1].Normal += normal;
		vertices[i2].Normal += normal;
	}

	for (uint32 i = 0; i < buffers->VerticesCount; i++)
	{
		vertices[i].Normal = Vec3::Normalize(vertices[i].Normal);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	UnbindMesh();
}

void BeginBatch(MeshBatch *batch, BATCH_TYPE type, uint32 maxCount, bool debug)
{
	batch->MaxCount = maxCount;

	if (batch->Type == BATCH_TYPE::UNKNOWN && type != BATCH_TYPE::UNKNOWN)
	{
		batch->Type = type;
	}

	if (type = BATCH_TYPE::SPRITE_BATCH)
	{
		if (debug)
		{
			if (!batch->Batch.VBO)
			{
				batch->Batch = CreateMesh(NULL, 4 * batch->MaxCount, NULL, 8 * batch->MaxCount, false, true);
			}

			BindMesh(&batch->Batch);

			//Store the data in the buffer
			glBufferData(GL_ARRAY_BUFFER, 4 * batch->MaxCount * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

			//Store the data in the buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * batch->MaxCount * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
		}
		else
		{
			if (!batch->Batch.VBO)
			{
				batch->Batch = CreateMesh(NULL, 4 * batch->MaxCount, NULL, 6 * batch->MaxCount, false, true);
			}

			BindMesh(&batch->Batch);

			//Store the data in the buffer
			glBufferData(GL_ARRAY_BUFFER, 4 * batch->MaxCount * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

			//Store the data in the buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * batch->MaxCount * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
		}
	}
	else
	{
		if (!batch->Batch.VBO)
		{
			batch->Batch = CreateMesh(NULL, 24 * batch->MaxCount, NULL, 36 * batch->MaxCount, false, true);
		}

		BindMesh(&batch->Batch);

		//Store the data in the buffer
		glBufferData(GL_ARRAY_BUFFER, 24 * batch->MaxCount * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

		//Store the data in the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * batch->MaxCount * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
	}
}

void PauseBatch(MeshBatch *batch)
{
	UnbindMesh();
}

void ResumeBatch(MeshBatch *batch)
{
	BindMesh(&batch->Batch);

	for (uint32 i = 0; i < batch->UsedSlotsCount; i++)
	{
		batch->TextureSlots[i] = 0;
	}

	batch->UsedSlotsCount = 0;
}

void AddSprite(MeshBatch *batch, vec3f pos, vec3f size, vec4f color, uint32 textureID, bool debug, mat4f model)
{
	uint32 vertexOffset = batch->CurrentSize * 4;
	uint32 indexOffset = batch->CurrentSize * 6;
	if (debug)
	{
		indexOffset = batch->CurrentSize * 8;
	}

	batch->CurrentSize++;

	float slotIndex = 0;
	/*
	bool found = false;

	for (uint32 i = 0; i < batch->UsedSlotsCount; i++)
	{
		if (batch->TextureSlots[i] == textureID)
		{
			found = true;
			slotIndex = i;
			break;
		}
	}

	if (!found)
	{
		if (batch->UsedSlotsCount >= 8)
		{
			PauseBatch(batch);
			RenderBatch(batch, debug);
			ResumeBatch(batch);
		}

		batch->TextureSlots[batch->UsedSlotsCount] = textureID;
		slotIndex = batch->UsedSlotsCount;
		batch->UsedSlotsCount++;
	}*/
	if (!debug)
	{
		{
			vec3f lowerLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
			vec3f upperLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);
			vec3f lowerRight = vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
			vec3f upperRight = vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);

			if (!batch->Stop)
			{
				lowerLeft = model * lowerLeft;
				upperLeft = model * upperLeft;
				lowerRight = model * lowerRight;
				upperRight = model * upperRight;
			}
			
			Vertex vertices[] =
			{
				Vertex{ lowerLeft, vec2f(0, 0), color, slotIndex },
				Vertex{ upperLeft, vec2f(0, 1), color, slotIndex },
				Vertex{ upperRight, vec2f(1, 1), color, slotIndex },
				Vertex{ lowerRight, vec2f(1, 0), color, slotIndex }
			};
			glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * sizeof(Vertex), 4 * sizeof(Vertex), vertices);
		}
	}
	else
	{
		vec3f lowerLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
		vec3f upperLeft = vec3f(pos.X - (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);
		vec3f lowerRight = vec3f(pos.X + (size.X / 2.0f), pos.Y - (size.Y / 2.0f), pos.Z);
		vec3f upperRight = vec3f(pos.X + (size.X / 2.0f), pos.Y + (size.Y / 2.0f), pos.Z);

		if (!batch->Stop)
		{
			lowerLeft = model * lowerLeft;
			upperLeft = model * upperLeft;
			lowerRight = model * lowerRight;
			upperRight = model * upperRight;
		}

		Vertex vertices[] =
		{
			Vertex{ lowerLeft, vec2f(0, 0), color },
			Vertex{ upperLeft, vec2f(0, 1), color },
			Vertex{ upperRight, vec2f(1, 1), color },
			Vertex{ lowerRight, vec2f(1, 0), color }
		};
		glBufferSubData(GL_ARRAY_BUFFER, vertexOffset * sizeof(Vertex), 4 * sizeof(Vertex), vertices);
	}

	if (debug)
	{
		uint32 indices[] =
		{
			0 + vertexOffset,
			1 + vertexOffset,

			1 + vertexOffset,
			2 + vertexOffset,

			2 + vertexOffset,
			3 + vertexOffset,

			3 + vertexOffset,
			0 + vertexOffset,
		};

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexOffset * sizeof(uint32), 8 * sizeof(uint32), indices);
	}
	else
	{
		uint32 indices[] =
		{
			0 + vertexOffset,
			1 + vertexOffset,
			3 + vertexOffset,

			1 + vertexOffset,
			2 + vertexOffset,
			3 + vertexOffset
		};

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexOffset * sizeof(uint32), 6 * sizeof(uint32), indices);
	}
}

void EndBatch(MeshBatch *batch, bool debug)
{
	UnbindMesh();
	RenderBatch(batch, debug);
	batch->CurrentSize = 0;
}

void RenderBatch(MeshBatch *batch, bool debug)
{
	/*for (uint32 i = 0; i < batch->UsedSlotsCount; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, batch->TextureSlots[i]);
	}*/

	if (debug)
	{
		glLineWidth(1.0f);
		BindMesh(&batch->Batch);
		glDrawElements(GL_LINES, batch->Batch.IndicesCount, GL_UNSIGNED_INT, NULL);
		UnbindMesh();
	}
	else
	{
		DrawMesh(&batch->Batch, false);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawSprite(Sprite *sprite)
{
	if (!sprite->Buffers.VBO)
	{
		sprite->Buffers = CreateSprite(sprite->Pos, sprite->Size, sprite->Skin.MeshColor, true, false);
	}
	BindTexture(&sprite->Skin.MeshTexture);
	DrawMesh(&sprite->Buffers, false);
	UnbindTexture();
}