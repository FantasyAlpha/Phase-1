#include <Components.h>

void RendererSystem::InitRendererSystem(uint32 count)
{
	Pool = Cinder::Memory::MemoryPool(sizeof(Renderable), count, false);
	Renderables = (Renderable *)Pool.Elements;
}

void RendererSystem::AddComponent(char *actorName, vec2f size, Material material, AnimationClip *clip)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(actorName);
	if (actorIndex < Pool.ChunkCount)
	{
		if (Pool.UsedHeaders[actorIndex].Current == Pool.ChunkCount + 1)
		{
			Cinder::Memory::PoolHeader block = Pool.Alloc();

			Renderables[block.Current].Size = size;
			Renderables[block.Current].RenderableMaterial = material;
			Renderables[block.Current].Clip = clip;
			if (!clip)
			{
				Renderables[block.Current].WithClip = false;
			}
			else
			{
				Renderables[block.Current].WithClip = true;
			}

			Renderables[block.Current].OwnerIndex = actorIndex;
			Owner_ComponentMap[actorName] = block.Current;
		}
	}	
}

uint32 RendererSystem::GetRenderableIndex(char *name)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);

	if (Owner->ActorManager.Pool.UsedHeaders[actorIndex].Current != Owner->ActorManager.Pool.ChunkCount + 1)
	{
		if (Owner_ComponentMap.find(name) != Owner_ComponentMap.end())
		{
			return Owner_ComponentMap[name];
		}
	}

	return Pool.ChunkCount + 1;
}

void RendererSystem::RemoveRenderer(char *actorName)
{
	uint32 index = GetRenderableIndex(actorName);
	if (index < Pool.ChunkCount)
	{
		Owner_ComponentMap.erase(actorName);
		Pool.Dealloc(index);
	}
}

void RendererSystem::RenderAllActive()
{
	ActivateShader(&MainShader);

	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::PROJECTION_MATRIX), 1, true, CalcProjection(&Owner->MainCamera).Elements_1D);
	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::VIEW_MATRIX), 1, true, CalcLookAtViewMatrix(&Owner->MainCamera).Elements_1D);
	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::MODEL_MATRIX), 1, GL_TRUE, mat4f().Elements_1D);

	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE0), 0);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE1), 1);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE2), 2);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE3), 3);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE4), 4);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE5), 5);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE6), 6);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE7), 7);

	uint32 index = Pool.FirstUsed;

	BeginBatch(&Renderer, BATCH_TYPE::SPRITE_BATCH, Owner_ComponentMap.size(), false);

	while (index != Pool.ChunkCount + 1)
	{
		uint32 ownerIndex = Renderables[index].OwnerIndex;

		AddSprite(&Renderer
			, Owner->ActorManager.Actors[ownerIndex].ModelMatrix
			, vec3f()
			, Renderables[index].Size
			, Renderables[index].RenderableMaterial.MeshColor
			, Renderables[index].RenderableMaterial.MeshTexture.TextureHandle
			, Renderables[index].Clip);

		index = Pool.UsedHeaders[index].Next;
	}

	EndBatch(&Renderer, false);
}

void RendererSystem::RenderDebugShapes()
{
	ActivateShader(&DebugShader);

	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::PROJECTION_MATRIX), 1, false, CalcProjection(&Owner->MainCamera).Elements_1D);
	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::VIEW_MATRIX), 1, false, CalcLookAtViewMatrix(&Owner->MainCamera).Elements_1D);
	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::MODEL_MATRIX), 1, false, mat4f().Elements_1D);

	uint32 index = Pool.FirstUsed;

	uint32 batchSize = Owner_ComponentMap.size() + Owner->CollisionManager.Owner_ComponentMap.size();

	BeginBatch(&DebugRenderer, BATCH_TYPE::SPRITE_BATCH, batchSize, true);

	while (index != Pool.ChunkCount + 1)
	{
		uint32 ownerIndex = Renderables[index].OwnerIndex;		

		vec4f c = vec4f{ 1, 0, 0, 1 };

		AddSprite(&DebugRenderer
			, Owner->ActorManager.Actors[ownerIndex].ModelMatrix
			, vec3f()
			, Renderables[index].Size
			, c
			, Renderables[index].RenderableMaterial.MeshTexture.TextureHandle
			, Renderables[index].Clip
			, true);

		index = Pool.UsedHeaders[index].Next;
	}

	index = Owner->CollisionManager.Pool.FirstUsed;

	while (index != Owner->CollisionManager.Pool.ChunkCount + 1)
	{
		uint32 ownerIndex = Owner->CollisionManager.Colliders[index].OwnerIndex;
		vec4f c = vec4f{ 0, 0, 1, 1 };

		AddSprite(&DebugRenderer
			, Owner->ActorManager.Actors[ownerIndex].ModelMatrix
			, vec3f()
			, Owner->CollisionManager.Colliders[index].size
			, c
			, 0
			, 0
			, true);

		index = Owner->CollisionManager.Pool.UsedHeaders[index].Next;
	}

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
	AddUniform(&MainShader, UNIFORMS::MOUSE_POS, "mousePos");
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

	AddUniform(&DebugShader, UNIFORMS::MOUSE_POS, "mousePos");
}

Renderable* RendererSystem::GetRenderable(char *name)
{
	uint32 index = GetRenderableIndex(name);

	if (index < Pool.ChunkCount)
	{
		return &Renderables[index];
	}

	return NULL;
}