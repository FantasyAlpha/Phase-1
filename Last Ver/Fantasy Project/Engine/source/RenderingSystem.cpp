#include <Components.h>

void RendererSystem::InitRendererSystem(uint32 count)
{
	StaticsPool = Cinder::Memory::MemoryPool(sizeof(Renderable), count, false);
	MovablesPool = Cinder::Memory::MemoryPool(sizeof(Renderable), count, false);
	StaticRenderables = (Renderable *)StaticsPool.Elements;
	MovableRenderables = (Renderable *)MovablesPool.Elements;
	MainLight.Strength = 0;
	MainLight.Color = vec4f(1, 1, 1, 1);
}

void RendererSystem::AddComponent(char *name, RenderableType type, vec3f pos, vec2f size, Material material, AnimationClip *clip)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);

	if (type == RenderableType::Static)
	{
		if (actorIndex < StaticsPool.ChunkCount)
		{
			uint32 index = GetRenderableIndex(name, type);
			if (index == StaticsPool.ChunkCount + 1)
			{
				Cinder::Memory::PoolHeader block = StaticsPool.Alloc();

				StaticRenderables[block.Current].Size = size;
				StaticRenderables[block.Current].Position = pos;

				StaticRenderables[block.Current].RenderableMaterial = material;
				StaticRenderables[block.Current].Clip = clip;
				if (!clip)
				{
					StaticRenderables[block.Current].WithClip = false;
				}
				else
				{
					StaticRenderables[block.Current].WithClip = true;
				}

				StaticRenderables[block.Current].OwnerIndex = actorIndex;
				Owner_StaticComponentMap[name] = block.Current;
			}
		}
	}
	else
	{
		if (actorIndex < MovablesPool.ChunkCount)
		{
			uint32 index = GetRenderableIndex(name, type);
			if (index == MovablesPool.ChunkCount + 1)
			{
				Cinder::Memory::PoolHeader block = MovablesPool.Alloc();

				MovableRenderables[block.Current].Size = size;

				MovableRenderables[block.Current].RenderableMaterial = material;
				MovableRenderables[block.Current].Clip = clip;
				if (!clip)
				{
					MovableRenderables[block.Current].WithClip = false;
				}
				else
				{
					MovableRenderables[block.Current].WithClip = true;
				}

				MovableRenderables[block.Current].OwnerIndex = actorIndex;
				MovableRenderables[block.Current].Buffers = CreateSprite(vec3f(), size, material.MeshColor, clip, false);
				MovableRenderables[block.Current].DebugBuffers = CreateSprite(vec3f(), size, material.MeshColor, clip, true);

				Owner_MovableComponentMap[name] = block.Current;
			}
		}
	}	
}

uint32 RendererSystem::GetRenderableIndex(char *name, RenderableType type)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);

	if (type == RenderableType::Static)
	{
		if (Owner->ActorManager.Pool.UsedHeaders[actorIndex].Current != Owner->ActorManager.Pool.ChunkCount + 1)
		{
			if (Owner_StaticComponentMap.find(name) != Owner_StaticComponentMap.end())
			{
				return Owner_StaticComponentMap[name];
			}
		}

		return StaticsPool.ChunkCount + 1;
	}
	else
	{
		if (Owner->ActorManager.Pool.UsedHeaders[actorIndex].Current != Owner->ActorManager.Pool.ChunkCount + 1)
		{
			if (Owner_MovableComponentMap.find(name) != Owner_MovableComponentMap.end())
			{
				return Owner_MovableComponentMap[name];
			}
		}

		return MovablesPool.ChunkCount + 1;
	}	
}

void RendererSystem::RemoveRenderer(char *actorName, RenderableType type)
{
	uint32 index = GetRenderableIndex(actorName, type);

	if (type == RenderableType::Static)
	{
		if (index < StaticsPool.ChunkCount)
		{
			Owner_StaticComponentMap.erase(actorName);
			StaticsPool.Dealloc(index);
		}
	}
	else
	{
		if (index < MovablesPool.ChunkCount)
		{
			glDeleteBuffers(1, &MovableRenderables[index].Buffers.VBO);
			glDeleteBuffers(1, &MovableRenderables[index].Buffers.EBO);
			Owner_MovableComponentMap.erase(actorName);
			MovablesPool.Dealloc(index);
		}
	}	
}

void RendererSystem::RenderAllActive()
{
	ActivateShader(&MainShader);

	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::PROJECTION_MATRIX), 1, true, CalcProjection(&Owner->MainCamera).Elements_1D);
	//glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::VIEW_MATRIX), 1, 0, CalcLookAtViewMatrix(&Owner->MainCamera).Elements_1D);
	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::MODEL_MATRIX), 1, GL_TRUE, mat4f().Elements_1D);

	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE0), 0);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE1), 1);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE2), 2);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE3), 3);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE4), 4);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE5), 5);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE6), 6);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE7), 7);

	glUniform3f(GetUniformLocation(&MainShader, UNIFORMS::LIGHT_POS), MainLight.Position.X, MainLight.Position.Y, MainLight.Position.Z);
	glUniform4f(GetUniformLocation(&MainShader, UNIFORMS::AMBIENT_COLOR), MainLight.Color.R, MainLight.Color.G, MainLight.Color.B, MainLight.Color.A);
	glUniform1f(GetUniformLocation(&MainShader, UNIFORMS::AMBIENT_STRENGTH), MainLight.Strength);

	uint32 index = StaticsPool.FirstUsed;

	BeginBatch(&Renderer, BATCH_TYPE::SPRITE_BATCH, Owner_StaticComponentMap.size(), false);

	while (index != StaticsPool.ChunkCount + 1)
	{
		uint32 ownerIndex = StaticRenderables[index].OwnerIndex;

		AddSprite(&Renderer
			, StaticRenderables[index].Position
			, StaticRenderables[index].Size
			, StaticRenderables[index].RenderableMaterial.MeshColor
			, StaticRenderables[index].RenderableMaterial.MeshTexture.TextureHandle
			, StaticRenderables[index].Clip);

		index = StaticsPool.UsedHeaders[index].Next;
	}

	EndBatch(&Renderer, false);

	index = MovablesPool.FirstUsed;

	while (index != MovablesPool.ChunkCount + 1)
	{
		uint32 ownerIndex = MovableRenderables[index].OwnerIndex;
		glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::MODEL_MATRIX), 1, GL_TRUE, Owner->ActorManager.Actors[ownerIndex].ModelMatrix.Elements_1D);

		if (MovableRenderables[index].Clip)
		{
			//AnimateSprite(&MovableRenderables[index].Buffers, MovableRenderables[index].Clip);
		}

		BindTexture(&MovableRenderables[index].RenderableMaterial.MeshTexture);
		GLenum g = glGetError();
		DrawMesh(&MovableRenderables[index].Buffers, false);
		UnbindTexture();

		index = MovablesPool.UsedHeaders[index].Next;
	}
}

void RendererSystem::UpdateRenderables()
{
	uint32 index = StaticsPool.FirstUsed;

	BeginBatch(&Renderer, BATCH_TYPE::SPRITE_BATCH, Owner_StaticComponentMap.size(), false);

	while (index != StaticsPool.ChunkCount + 1)
	{
		uint32 ownerIndex = StaticRenderables[index].OwnerIndex;

		if (StaticRenderables[index].Clip)
		{
			StaticRenderables[index].Clip->UPS = 1.0f / Owner->Delta;
		}

		AddSprite(&Renderer
			, StaticRenderables[index].Position
			, StaticRenderables[index].Size
			, StaticRenderables[index].RenderableMaterial.MeshColor
			, StaticRenderables[index].RenderableMaterial.MeshTexture.TextureHandle
			, StaticRenderables[index].Clip);

		index = StaticsPool.UsedHeaders[index].Next;
	}

	UnbindMesh();

	index = MovablesPool.FirstUsed;

	while (index != MovablesPool.ChunkCount + 1)
	{
		uint32 ownerIndex = MovableRenderables[index].OwnerIndex;

		if (MovableRenderables[index].Clip)
		{
			MovableRenderables[index].Clip->UPS = 1.0f / Owner->Delta;
			AnimateSprite(&MovableRenderables[index].Buffers, MovableRenderables[index].Clip);
		}

		index = MovablesPool.UsedHeaders[index].Next;
	}
}

void RendererSystem::RenderDebugShapes()
{
	ActivateShader(&DebugShader);

	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::PROJECTION_MATRIX), 1, 1, CalcProjection(&Owner->MainCamera).Elements_1D);
	//glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::VIEW_MATRIX), 1, 1, CalcLookAtViewMatrix(&Owner->MainCamera).Elements_1D);

	uint32 index = 0;

	uint32 batchSize = Owner_StaticComponentMap.size() + Owner->CollisionManager.Owner_ComponentMap.size() + 1;

	index = MovablesPool.FirstUsed;

	while (index != MovablesPool.ChunkCount + 1)
	{
		uint32 ownerIndex = MovableRenderables[index].OwnerIndex;
		glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::MODEL_MATRIX), 1, GL_TRUE, Owner->ActorManager.Actors[ownerIndex].ModelMatrix.Elements_1D);
		vec4f color = vec4f{ 1, 0, 1, 1 };

		EditSprite(&MovableRenderables[index].DebugBuffers, vec3f(), MovableRenderables[index].Size, color);

		DrawMesh(&MovableRenderables[index].DebugBuffers, true);

		index = MovablesPool.UsedHeaders[index].Next;
	}

	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::MODEL_MATRIX), 1, 1, mat4f().Elements_1D);

	BeginBatch(&DebugRenderer, BATCH_TYPE::SPRITE_BATCH, batchSize, true);
	index = StaticsPool.FirstUsed;

	while (index != StaticsPool.ChunkCount + 1)
	{
		uint32 ownerIndex = StaticRenderables[index].OwnerIndex;		

		vec4f c = vec4f{ 1, 0, 0, 1 };

		AddSprite(&DebugRenderer
			, StaticRenderables[index].Position
			, StaticRenderables[index].Size
			, c
			, StaticRenderables[index].RenderableMaterial.MeshTexture.TextureHandle
			, StaticRenderables[index].Clip			
			, true
			, Owner->ActorManager.Actors[ownerIndex].ModelMatrix);

		index = StaticsPool.UsedHeaders[index].Next;
	}

	index = Owner->CollisionManager.Pool.FirstUsed;

	while (index != Owner->CollisionManager.Pool.ChunkCount + 1)
	{
		uint32 ownerIndex = Owner->CollisionManager.Colliders[index].OwnerIndex;
		char *name = Owner->CollisionManager.Colliders[index].OwnerName;
		vec4f color = vec4f{ 0, 0, 1, 1 };
		if (GetRenderableIndex(name, RenderableType::Static) != StaticsPool.ChunkCount + 1)
		{
			AddSprite(&DebugRenderer
				, *Owner->CollisionManager.Colliders[index].pos
				, Owner->CollisionManager.Colliders[index].size
				, color
				, 0
				, 0
				, true
				, mat4f());
		}
		else
		{
			AddSprite(&DebugRenderer
				, *Owner->CollisionManager.Colliders[index].pos
				, Owner->CollisionManager.Colliders[index].size
				, color
				, 0
				, 0
				, true
				, mat4f());
		}		

		index = Owner->CollisionManager.Pool.UsedHeaders[index].Next;
	}

	vec4f color = vec4f{ 0, 1, 1, 1 };

	AddSprite(&DebugRenderer
		, Owner->MainCamera.Eye
		, Owner->MainCamera.WindowSize
		, color
		, 0
		, 0
		, true);

	EndBatch(&DebugRenderer, true);	
}

void RendererSystem::InitMainShader(char *vertexShader, char *fragmentShader)
{
	char **attributelocations = 0;
	attributelocations = new char*[4];
	attributelocations[0] = "position";
	attributelocations[1] = "inputTexCoords";
	attributelocations[2] = "color";
	attributelocations[3] = "slotIndex";
	MainShader = CreateShader(vertexShader, fragmentShader, attributelocations, 4);

	AddUniform(&MainShader, UNIFORMS::MODEL_MATRIX, "modelMatrix");
	AddUniform(&MainShader, UNIFORMS::VIEW_MATRIX, "viewMatrix");
	AddUniform(&MainShader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");
	AddUniform(&MainShader, UNIFORMS::TEXTURE0, "myTexture0");
	AddUniform(&MainShader, UNIFORMS::TEXTURE1, "myTexture1");
	AddUniform(&MainShader, UNIFORMS::TEXTURE2, "myTexture2");
	AddUniform(&MainShader, UNIFORMS::TEXTURE3, "myTexture3");
	AddUniform(&MainShader, UNIFORMS::TEXTURE4, "myTexture4");
	AddUniform(&MainShader, UNIFORMS::TEXTURE5, "myTexture5");
	AddUniform(&MainShader, UNIFORMS::TEXTURE6, "myTexture6");
	AddUniform(&MainShader, UNIFORMS::TEXTURE7, "myTexture7");
	AddUniform(&MainShader, UNIFORMS::LIGHT_POS, "lightPos");
	AddUniform(&MainShader, UNIFORMS::AMBIENT_COLOR, "ambientColor");
	AddUniform(&MainShader, UNIFORMS::AMBIENT_STRENGTH, "ambientStrength");
}

void RendererSystem::InitDebugShader(char *vertexShader, char *fragmentShader)
{
	char **attributelocations = 0;
	attributelocations = new char*[4];
	attributelocations[0] = "position";
	attributelocations[1] = "inputTexCoords";
	attributelocations[2] = "color";
	attributelocations[3] = "slotIndex";
	DebugShader = CreateShader(vertexShader, fragmentShader, attributelocations, 4);

	AddUniform(&DebugShader, UNIFORMS::MODEL_MATRIX, "modelMatrix");
	AddUniform(&DebugShader, UNIFORMS::VIEW_MATRIX, "viewMatrix");
	AddUniform(&DebugShader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");
}

Renderable* RendererSystem::GetRenderable(char *name, RenderableType type)
{
	uint32 index = GetRenderableIndex(name, type);
	if (type == RenderableType::Static)
	{
		if (index < StaticsPool.ChunkCount)
		{
			return &StaticRenderables[index];
		}
	}
	else
	{
		if (index < MovablesPool.ChunkCount)
		{
			return &MovableRenderables[index];
		}
	}	

	return NULL;
}