#include <Hover.h>

Hover::Hover(char *name)
{
	Name = name;
	Type = "HOVER";
	Size = sizeof(Hover);
	ButtonRenderers = new std::vector<SpriteRenderer *>();
	CurrentRenderer = 0;
}

void Hover::Destroy()
{
	delete ButtonRenderers;
}

void Hover::Update(float delta, Game_Input *input)
{
	uint32 curr = CurrentRenderer;
	if (input->UP.KeyUp)
	{
		if (CurrentRenderer == 0)
		{
			CurrentRenderer = ButtonRenderers->size() - 1;
		}
		else
		{
			CurrentRenderer--;
		}
	}

	if (input->DOWN.KeyUp)
	{
		if (CurrentRenderer == ButtonRenderers->size() - 1)
		{
			CurrentRenderer = 0;
		}
		else
		{
			CurrentRenderer++;
		}
	}

	if (curr == CurrentRenderer)
	{
		EditSprite(&(*ButtonRenderers)[CurrentRenderer]->SpriteData.Buffers
			, (*ButtonRenderers)[CurrentRenderer]->SpriteData.Pos
			, (*ButtonRenderers)[CurrentRenderer]->SpriteData.Size
			, (*ButtonRenderers)[curr]->SpriteData.Skin.MeshColor * vec4f(0.5f, 0.5f, 0.5f, 1));
	}
	else
	{
		EditSprite(&(*ButtonRenderers)[curr]->SpriteData.Buffers
			, (*ButtonRenderers)[curr]->SpriteData.Pos
			, (*ButtonRenderers)[curr]->SpriteData.Size
			, (*ButtonRenderers)[curr]->SpriteData.Skin.MeshColor);
	}
}

void Hover::AddButton(SpriteRenderer *renderer)
{
	ButtonRenderers->push_back(renderer);
}

char* Hover::GetCurrentButton()
{
	return (*ButtonRenderers)[CurrentRenderer]->Name;
}

void Hover::Reset()
{
	uint32 curr = CurrentRenderer;

	EditSprite(&(*ButtonRenderers)[curr]->SpriteData.Buffers
		, (*ButtonRenderers)[curr]->SpriteData.Pos
		, (*ButtonRenderers)[curr]->SpriteData.Size
		, (*ButtonRenderers)[curr]->SpriteData.Skin.MeshColor);

	CurrentRenderer = 0;

	EditSprite(&(*ButtonRenderers)[CurrentRenderer]->SpriteData.Buffers
		, (*ButtonRenderers)[CurrentRenderer]->SpriteData.Pos
		, (*ButtonRenderers)[CurrentRenderer]->SpriteData.Size
		, (*ButtonRenderers)[curr]->SpriteData.Skin.MeshColor * vec4f(0.5f, 0.5f, 0.5f, 0.5f));
}
