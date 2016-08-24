#include "RenderingEngine.h"
#include "SceneManager.h"
#include "SpriteRenderer.h"

void RenderingEngine::Init()
{
	
}

void RenderingEngine::Render()
{
	std::vector<GameComponent *> *renderers = Scene->GetAllComponents("RENDERABLE");
	
	if (renderers)
	{
		ActivateShader(&MainShader);

		glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::PROJECTION_MATRIX), 1, true, CalcProjection(&Scene->MainCamera).Elements_1D);
		glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::VIEW_MATRIX), 1, true, CalcLookAtViewMatrix(&Scene->MainCamera).Elements_1D);
		
		glUniform1i(GetUniformLocation(&MainShader, UNIFORMS::TEXTURE0), 0);
	
		for (uint32 i = 0; i < renderers->size(); i++)
		{
			SpriteRenderer *renderer = (SpriteRenderer *)(*renderers)[i];

			if (Scene->CurrentRoot)
			{
				if (renderer->Owner->CurrentRoot != Scene->CurrentRoot->Name
					|| renderer->Owner->CurrentRoot != Scene->CurrentRoot->Name)
				{
					continue;
				}
			}		

			if (!renderer->Owner->Enabled)
			{
				continue;
			}

			glUniformMatrix4fv(GetUniformLocation(&MainShader, UNIFORMS::MODEL_MATRIX), 1, true, renderer->Owner->ObjectTransform.ModelMatrix().Elements_1D);
			renderer->Render();
		}
	}	
}

void RenderingEngine::DebugRender()
{
	//std::vector<GameComponent *> *renderers = Scene->GetAllComponents("RENDERABLE");
	std::vector<GameComponent *> *colliders = Scene->GetAllComponents("COLLIDER");

	ActivateShader(&DebugShader);

	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::PROJECTION_MATRIX), 1, true, CalcProjection(&Scene->MainCamera).Elements_1D);
	glUniformMatrix4fv(GetUniformLocation(&DebugShader, UNIFORMS::MODEL_MATRIX), 1, true, mat4f().Elements_1D);
	
	uint32 totalCount = 0;
	/*
	if (renderers)
	{
		totalCount += renderers->size();
	}
	*/

	if (colliders)
	{
		totalCount += colliders->size();
	}

	BeginBatch(&this->DebugBatch, SPRITE_BATCH, totalCount, true);
	
	if (colliders)
	{
		for (uint32 i = 0; i < colliders->size(); i++)
		{
			Collider *collider = (Collider *)(*colliders)[i];
			
			if (Scene->CurrentRoot)
			{
				if (collider->Owner->CurrentRoot != Scene->CurrentRoot->Name)
				{
					continue;
				}
			}

			if (!collider->Owner->Enabled)
			{
				continue;
			}

			DebugBatch.Stop = false;
			AddSprite(&DebugBatch
				, *collider->pos + vec3f(collider->offset.X, collider->offset.Y, 0)
				, collider->size
				, vec4f(0, 0, 1, 1)
				, 0
				, true
				, mat4f());
		}
	}
	/*
	if (renderers)
	{
		for (uint32 i = 0; i < renderers->size(); i++)
		{
			SpriteRenderer *renderer = (SpriteRenderer *)(*renderers)[i];

			if (Scene->CurrentRoot)
			{
				if (renderer->Owner->CurrentRoot != Scene->CurrentRoot->Name)
				{
					continue;
				}
			}			

			DebugBatch.Stop = false;
			AddSprite(&DebugBatch
				, renderer->SpriteData.Pos
				, renderer->SpriteData.Size
				, vec4f(1, 0, 0, 1)
				, renderer->SpriteData.Skin.MeshTexture.TextureHandle
				, true
				, renderer->Owner->ObjectTransform.ModelMatrix());
		}
	}
	
	*/
	EndBatch(&DebugBatch, true);
}

void RenderingEngine::Update()
{
	DebugRender();
}

void RenderingEngine::InitShader(char *vertexShader, char *fragmentShader)
{
	char **attributelocations = 0;
	attributelocations = new char*[4];
	attributelocations[0] = "position";
	attributelocations[1] = "inputTexCoords";
	attributelocations[2] = "color";
	attributelocations[3] = "slot";
	attributelocations[4] = "normal";
	
	MainShader = CreateShader(vertexShader, fragmentShader, attributelocations, 4);

	AddUniform(&MainShader, UNIFORMS::MODEL_MATRIX, "modelMatrix");
	AddUniform(&MainShader, UNIFORMS::VIEW_MATRIX, "viewMatrix");
	AddUniform(&MainShader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");

	AddUniform(&MainShader, UNIFORMS::TEXTURE0, "myTexture0");
}

void RenderingEngine::InitDebugShader(char *vertexShader, char *fragmentShader)
{
	char **attributelocations = 0;
	attributelocations = new char*[4];
	attributelocations[0] = "position";
	attributelocations[1] = "inputTexCoords";
	attributelocations[2] = "color";
	attributelocations[3] = "slot";
	attributelocations[4] = "normal";

	DebugShader = CreateShader(vertexShader, fragmentShader, attributelocations, 4);

	AddUniform(&DebugShader, UNIFORMS::MODEL_MATRIX, "modelMatrix");
	AddUniform(&DebugShader, UNIFORMS::VIEW_MATRIX, "viewMatrix");
	AddUniform(&DebugShader, UNIFORMS::PROJECTION_MATRIX, "projectionMatrix");
}