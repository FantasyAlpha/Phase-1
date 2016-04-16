#include <Components.h>
#include <World.h>

void RendererSystem::InitRendererSystem(StackAllocator *sourceAllocator)
{
	MaxSize = TOTAL_SPRITES_SIZE;
	InitPartialPoolSystem(sourceAllocator, &Allocator, MaxSize, SPRITE_RENDERER_SIZE);
	Renderers = (Renderer*)Allocator.Blocks->Dimensions.BaseAddress;
}

void RendererSystem::AddComponent(char *actorName, Renderer renderer)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(actorName);
	if (actorIndex < MAX_ACTOR_COUNT)
	{
		if (!Allocator.Blocks[actorIndex].IsUsed)
		{
			PoolBlock block = PoolAlloc(&Allocator);

			Renderers[block.BlockIndex] = renderer;
			Renderers[block.BlockIndex].OwnerIndex = actorIndex;
			Owner_ComponentMap[actorName] = block.BlockIndex;
		}
	}	
}

uint32 RendererSystem::GetRendererIndex(char *name)
{
	uint32 actorIndex = Owner->ActorManager.GetActorIndex(name);

	if (Allocator.Blocks[actorIndex].IsUsed)
	{
		if (Owner_ComponentMap.find(name) != Owner_ComponentMap.end())
		{
			return Owner_ComponentMap[name];
		}
	}

	return MAX_ACTOR_COUNT + 1;
}

void RendererSystem::RemoveRenderer(char *actorName)
{
	uint32 index = GetRendererIndex(actorName);
	if (index < MAX_ACTOR_COUNT)
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

	uint32 index = Allocator.FirstUsed;

	while (index != Allocator.BlockCount + 1)
	{
		uint32 ownerIndex = Renderers[index].OwnerIndex;
		glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::MODEL_MATRIX), 1, GL_TRUE, Owner->TransformManager.Transforms[ownerIndex].ModelMatrix.GetElemets());
		glUniform4f(GetUniformLocation(&MainShader, UNIFORMS::COLOR)
			, Renderers[index].RenderableMaterial.MeshColor.red
			, Renderers[index].RenderableMaterial.MeshColor.green
			, Renderers[index].RenderableMaterial.MeshColor.blue
			, Renderers[index].RenderableMaterial.MeshColor.alpha);

		glUniform4f(GetUniformLocation(&MainShader, UNIFORMS::AMBIENT_COLOR)
			, Renderers[index].RenderableMaterial.BaseLight.LightColor.red
			, Renderers[index].RenderableMaterial.BaseLight.LightColor.green
			, Renderers[index].RenderableMaterial.BaseLight.LightColor.blue
			, Renderers[index].RenderableMaterial.BaseLight.LightColor.alpha);

		glUniform1f(GetUniformLocation(&MainShader, UNIFORMS::AMBIENT_STRENGTH)
			, Renderers[index].RenderableMaterial.BaseLight.LightStrength);


		BindTexture(&Renderers[index].RenderableMaterial.MeshTexture);
		DrawMesh(&Renderers[index].Renderable);
		UnbindTexture();
		index = Allocator.Blocks[index].NextUsedIndex;
	}
}

void RenderSingleRenderer(Renderer *renderer, Shader *shader, Transform *transform, Camera *camera)
{
	ActivateShader(shader);

	glUniformMatrix4fv(GetUniformLocation(shader, UNIFORMS::PROJECTION_MATRIX), 1, true, CalcProjection(camera).GetElemets());
	glUniformMatrix4fv(GetUniformLocation(shader, UNIFORMS::VIEW_MATRIX), 1, true, CalcLookAtViewMatrix(camera).GetElemets());
	glUniformMatrix4fv(GetUniformLocation(shader, UNIFORMS::MODEL_MATRIX), 1, true, CalcModelMatrix(transform).GetElemets());
	glUniform4f(GetUniformLocation(shader, UNIFORMS::COLOR)
		, renderer->RenderableMaterial.MeshColor.red
		, renderer->RenderableMaterial.MeshColor.green
		, renderer->RenderableMaterial.MeshColor.blue
		, renderer->RenderableMaterial.MeshColor.alpha);

	glUniform4f(GetUniformLocation(shader, UNIFORMS::AMBIENT_COLOR)
		,  renderer->RenderableMaterial.BaseLight.LightColor.red
		,  renderer->RenderableMaterial.BaseLight.LightColor.green
		,  renderer->RenderableMaterial.BaseLight.LightColor.blue
		,  renderer->RenderableMaterial.BaseLight.LightColor.alpha);

	glUniform1f(GetUniformLocation(shader, UNIFORMS::AMBIENT_STRENGTH)
		, renderer->RenderableMaterial.BaseLight.LightStrength);

	BindTexture(&renderer->RenderableMaterial.MeshTexture);
	DrawMesh(&renderer->Renderable);
	UnbindTexture();
}

void RendererSystem::InitMainShader(char *vertexShader, char *fragmentShader)
{
	MainShader = CreateShader(vertexShader, fragmentShader);

	AddUniform(&MainShader, UNIFORMS::MODEL_MATRIX, "modelMatrix");
	AddUniform(&MainShader, UNIFORMS::VIEW_MATRIX, "viewMatrix");
	AddUniform(&MainShader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");
	AddUniform(&MainShader, UNIFORMS::COLOR, "myColor");
	AddUniform(&MainShader, UNIFORMS::AMBIENT_COLOR, "ambientColor");
	AddUniform(&MainShader, UNIFORMS::AMBIENT_STRENGTH, "ambientStrength");
}