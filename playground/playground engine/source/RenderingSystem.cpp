#include <Components.h>
#include <World.h>

void RendererSystem::InitRendererSystem(StackAllocator *sourceAllocator)
{
	MaxSize = TOTAL_SPRITES_SIZE;
	InitPartialPoolSystem(sourceAllocator, &Allocator, MaxSize, SPRITE_RENDERER_SIZE);
	Renderables = (Renderable*)Allocator.Blocks->Dimensions.BaseAddress;
}

void RendererSystem::AddComponent(char *actorName, vec3f pos, vec2 size, Material material, AnimationClip *clip)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(actorName);
	if (actorIndex < MAX_ACTOR_COUNT)
	{
		if (!Allocator.Blocks[actorIndex].IsUsed)
		{
			PoolBlock block = PoolAlloc(&Allocator);

			Renderables[block.BlockIndex].Position = pos;
			Renderables[block.BlockIndex].Size = size;
			Renderables[block.BlockIndex].RenderableMaterial = material;
			Renderables[block.BlockIndex].Clip = clip;
			if (!clip)
			{
				Renderables[block.BlockIndex].WithClip = false;
			}
			else
			{
				Renderables[block.BlockIndex].WithClip = true;
			}

			Renderables[block.BlockIndex].OwnerIndex = actorIndex;
			Owner_ComponentMap[actorName] = block.BlockIndex;
		}
	}	
}

uint32 RendererSystem::GetRenderableIndex(char *name)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);

	if (Owner->ActorManager.Allocator.Blocks[actorIndex].IsUsed)
	{
		if (Owner_ComponentMap.find(name) != Owner_ComponentMap.end())
		{
			return Owner_ComponentMap[name];
		}
	}

	return MAX_SPRITE_COUNT + 1;
}

void RendererSystem::RemoveRenderer(char *actorName)
{
	uint32 index = GetRenderableIndex(actorName);
	if (index < MAX_SPRITE_COUNT)
	{
		Owner_ComponentMap.erase(actorName);
		PoolDealloc(&Allocator, index);
	}
}

void RendererSystem::RenderAllActive()
{
	ActivateShader(&MainShader);

	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::PROJECTION_MATRIX), 1, true, CalcProjection(&Owner->MainCamera).GetElemets());
	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::VIEW_MATRIX), 1, true, CalcLookAtViewMatrix(&Owner->MainCamera).GetElemets());

	glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::MODEL_MATRIX), 1, GL_TRUE,mat4().GetElemets());

	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE0), 0);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE1), 1);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE2), 2);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE3), 3);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE4), 4);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE5), 5);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE6), 6);
	glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE7), 7);

	uint32 index = Allocator.FirstUsed;

	BeginBatch(&Renderer.RenderableBatch, BATCH_TYPE::SPRITE_BATCH, Owner_ComponentMap.size(), false);

	while (index != Allocator.BlockCount + 1)
	{
		uint32 ownerIndex = Renderables[index].OwnerIndex;

		mat4 model = Owner->TransformManager.Transforms[ownerIndex].ModelMatrix;

		vec3f pos = model *Renderables[index].Position;

		//Mesh m = CreateSprite(Renderables[index].Position + (vec3f(1, 1, 0) * index * 51), vec2(50, 50), Color());

		/*glUniform4f(GetUniformLocation(&MainShader, UNIFORMS::AMBIENT_COLOR)
		, Renderables[index].RenderableMaterial.BaseLight.LightColor.red
		, Renderables[index].RenderableMaterial.BaseLight.LightColor.green
		, Renderables[index].RenderableMaterial.BaseLight.LightColor.blue
		, Renderables[index].RenderableMaterial.BaseLight.LightColor.alpha);

		glUniform1f(GetUniformLocation(&MainShader, UNIFORMS::AMBIENT_STRENGTH)
		, Renderables[index].RenderableMaterial.BaseLight.LightStrength);
		*/		

		AddSprite(&Renderer.RenderableBatch
			,pos
			, Renderables[index].Size
			, Renderables[index].RenderableMaterial.MeshColor
			, Renderables[index].RenderableMaterial.MeshTexture.TextureHandle
			, Renderables[index].Clip);

		index = Allocator.Blocks[index].NextUsedIndex;
	}

	EndBatch(&Renderer.RenderableBatch, false);
}

void RendererSystem::RenderDebugShapes()
{
	ActivateShader(&DebugShader);

	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::PROJECTION_MATRIX), 1, true, CalcProjection(&Owner->MainCamera).GetElemets());
	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::VIEW_MATRIX), 1, true, CalcLookAtViewMatrix(&Owner->MainCamera).GetElemets());
	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::MODEL_MATRIX), 1, GL_TRUE, mat4().GetElemets());

	uint32 index = Allocator.FirstUsed;

	uint32 batchSize = Owner_ComponentMap.size() + Owner->CollisionManager.Owner_ComponentMap.size();

	BeginBatch(&DebugRenderer.RenderableBatch, BATCH_TYPE::SPRITE_BATCH, batchSize, true);

	while (index != Allocator.BlockCount + 1)
	{
		uint32 ownerIndex = Renderables[index].OwnerIndex;
		
		mat4 model = Owner->TransformManager.Transforms[ownerIndex].ModelMatrix;

		vec3f pos = model * Renderables[index].Position;

		Color c = Color{ 1, 0, 0, 1 };

		AddSprite(&DebugRenderer.RenderableBatch
			, pos
			, Renderables[index].Size
			, c
			, Renderables[index].RenderableMaterial.MeshTexture.TextureHandle
			, Renderables[index].Clip
			, true);

		index = Allocator.Blocks[index].NextUsedIndex;

	}

	index = Owner->CollisionManager.Allocator.FirstUsed;

	while (index != Owner->CollisionManager.Allocator.BlockCount + 1)
	{
		uint32 ownerIndex = Owner->CollisionManager.Colliders[index].OwnerIndex;
		Color c = Color{ 0, 0, 1, 1 };

		mat4 model = Owner->TransformManager.Transforms[ownerIndex].ModelMatrix;

		vec3f pos = model * Owner->TransformManager.Transforms[ownerIndex].CurrentTransform.Position;

		AddSprite(&DebugRenderer.RenderableBatch
			, pos
			, Owner->CollisionManager.Colliders[index].size
			, c
			, 0
			, 0
			, true);

		index = Owner->CollisionManager.Allocator.Blocks[index].NextUsedIndex;
	}

	EndBatch(&DebugRenderer.RenderableBatch, true);
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

	if (index < MAX_SPRITE_COUNT)
	{
		return &Renderables[index];
	}

	return NULL;
}